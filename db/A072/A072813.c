/*
 * A072813  a(n)^A072814(n) is the n-th perfect power.
 * https://oeis.org/A072813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the base of the n‑th perfect power value (values are
 *            distinct, sorted increasingly; for duplicate values the largest
 *            base is chosen). Example:
 *            1,4(2),8(2),9(3),16(4),25(5),27(3),32(2),
 * Range:     u8 none, u16 none, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes only standard headers that are available in C11 and C++17. */
#include <stdlib.h>

/* Helper structure for sorting. */
typedef struct {
    seq_t value;   /* perfect power value */
    seq_t base;    /* base producing that value */
} ppair_t;

/* Comparison function: primary key by value ascending,
   secondary key by base descending (so largest base first for duplicates). */
static int cmp_ppair(const void *p1, const void *p2)
{
    const ppair_t *a = (const ppair_t *)p1;
    const ppair_t *b = (const ppair_t *)p2;
    if (a->value < b->value) return -1;
    if (a->value > b->value) return 1;
    /* same value – keep larger base first */
    if (a->base > b->base) return -1;
    if (a->base < b->base) return 1;
    return 0;
}

/* Static data holding the pre‑computed sequence. */
static seq_t *bases = NULL;          /* array of bases: bases[i] = a(i+1) */
static idx_t    base_count = 0;      /* number of stored terms */
static int      initialized = 0;     /* flag */

/* Generates the first enough terms (at least ~33000) of the sequence
   by enumerating all perfect powers up to a chosen limit. */
static void init_sequence(void)
{
    /* Choose a limit large enough to produce far more than 10000 terms. */
    const idx_t LIMIT = 1000000000ULL;  /* 1e9 */

    /* Rough upper bound on number of generated pairs: ~70 000. */
    size_t temp_cap = 80000;
    ppair_t *temp = (ppair_t *)malloc(temp_cap * sizeof(ppair_t));
    size_t temp_count = 0;

    /* Enumerate b^e for b ≥ 2, e ≥ 2, stopping when the value exceeds LIMIT. */
    for (idx_t base = 2; ; ++base) {
        seq_t val = (seq_t)base * base;   /* base^2 */
        if (val > LIMIT) break;           /* base^2 already too large */

        for (idx_t exp = 2; val <= LIMIT; ++exp) {
            /* Store the pair. */
            if (temp_count == temp_cap) {
                temp_cap <<= 1;
                temp = (ppair_t *)realloc(temp, temp_cap * sizeof(ppair_t));
            }
            temp[temp_count].value = val;
            temp[temp_count].base  = base;
            ++temp_count;

            /* Prepare for next exponent. */
            if (val > LIMIT / base) break;   /* would exceed LIMIT */
            val *= base;
        }
    }

    /* Sort the pairs (value ascending, base descending). */
    qsort(temp, temp_count, sizeof(ppair_t), cmp_ppair);

    /* Deduplicate, keeping the first occurrence (largest base). */
    base_count = temp_count + 1;          /* +1 for the value 1 (base 1) */
    bases = (seq_t *)malloc(base_count * sizeof(seq_t));
    bases[0] = (seq_t)1;                  /* a(1) = 1 */

    size_t idx = 1;                       /* index into bases array */
    for (size_t i = 0; i < temp_count; ++i) {
        /* Skip duplicates: they appear consecutively after sorting. */
        if (i > 0 && temp[i].value == temp[i - 1].value)
            continue;
        bases[idx++] = temp[i].base;
    }

    free(temp);
    initialized = 1;
}

seq_t a(idx_t n)
{
    if (!initialized) {
        init_sequence();
    }

    if (n == 0 || n > (idx_t)base_count) {
        /* Undefined for n outside the pre‑computed range – return 0. */
        return (seq_t)0;
    }

    return bases[n - 1];
}
