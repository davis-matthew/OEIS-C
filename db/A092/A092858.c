/*
 * A092858  "Sum" of the sequences of primes and the triangular numbers
 *          (A000217).
 * https://oeis.org/A092858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th element (offset 1) of the sorted union of all primes
 *            and all triangular * numbers T(k) = k·(k+1)/2, starting at 5
 *            (i.e., the first term 5). The sequence contains * every prime or
 *            triangular number greater than or equal to 5, with duplicates
 *            removed, * in increasing order
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* -------------------------------------------------------------------------- */
/* Configuration: the sieve limit is large enough to generate all primes  needed
 * for n up to about 10⁴.  With 200 000 the number of primes is 17 984, which is
 * more than enough for the required number of terms.                            */
/* -------------------------------------------------------------------------- */
static const idx_t PRIME_LIMIT = 200000;          /* maximum prime for sieve */
static uint64_t *primes = NULL;                  /* array of primes up to PRIME_LIMIT */
static size_t prime_count = 0;                   /* number of primes stored */

/* -------------------------------------------------------------------------- */
/* One‑time sieve; called lazily when needed.                               */
/* -------------------------------------------------------------------------- */
static void init_primes(void)
{
    static bool done = false;
    if (done) return;                      /* already done */
    done = true;

    /* Allocate the composite flag array.  Must cast the void* return value */
    bool *comp = (bool *)calloc((size_t)PRIME_LIMIT + 1, sizeof(bool));
    if (!comp) return;                     /* unlikely, but safe */

    for (idx_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) {
            for (idx_t j = i * i; j <= PRIME_LIMIT; j += i)
                comp[j] = true;
        }
    }

    /* Count primes and store them */
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i)
        if (!comp[i]) ++prime_count;

    primes = (uint64_t *)malloc(prime_count * sizeof(uint64_t));
    if (!primes) { free(comp); return; }

    size_t idx = 0;
    for (idx_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!comp[i]) primes[idx++] = (uint64_t)i;
    }

    free(comp);
}

/* -------------------------------------------------------------------------- */
/* Storage for already generated a(n) values.  The array contains only terms
 * >=5, in increasing order.                                                   */
/* -------------------------------------------------------------------------- */
static seq_t *a_vals = NULL;          /* dynamic array of generated terms */
static size_t a_cap  = 0;              /* allocated capacity */
static size_t a_len  = 0;              /* number of terms stored */

/* -------------------------------------------------------------------------- */
/* Pointers into the two sequences that are being merged in order.           */
/* -------------------------------------------------------------------------- */
static size_t prime_idx = 0;           /* index into primes array          */
static idx_t   tri_idx   = 1;          /* next triangular number index     */

/* -------------------------------------------------------------------------- */
/* Ensure that a_vals has at least 'needed' capacity.                         */
/* -------------------------------------------------------------------------- */
static void ensure_a_capacity(size_t needed)
{
    if (needed <= a_cap) return;
    size_t new_cap = a_cap ? a_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *newp = (seq_t *)realloc(a_vals, new_cap * sizeof(seq_t));
    a_vals = newp;
    a_cap  = new_cap;
}

/* -------------------------------------------------------------------------- */
/* Generate terms up to the specified target count (1‑based).                 */
/* -------------------------------------------------------------------------- */
static void generate_until(idx_t target)
{
    init_primes();                         /* guarantees primes[] ready */

    while (a_len < target) {
        seq_t prime_val = (prime_idx < prime_count) ? (seq_t)primes[prime_idx] : (seq_t)-1;
        seq_t tri_val   = (seq_t)tri_idx * ((seq_t)tri_idx + 1) / 2;

        seq_t next;
        if (prime_idx < prime_count && prime_val == tri_val) {
            next = prime_val;
            ++prime_idx;
            ++tri_idx;
        } else if (prime_idx < prime_count && prime_val < tri_val) {
            next = prime_val;
            ++prime_idx;
        } else {
            next = tri_val;
            ++tri_idx;
        }

        if (next >= 5) {                     /* skip 1,2,3 */
            ensure_a_capacity(a_len + 1);
            a_vals[a_len++] = next;
        }
    }
}

/* -------------------------------------------------------------------------- */
/* Public entry point: return the n‑th term of the sequence.                 */
/* -------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;              /* undefined for n=0, harmless */
    generate_until(n);                       /* fill up to n terms */
    return a_vals[n - 1];
}
