/*
 * A025334  Numbers that are the sum of 3 nonzero squares in 6 or more ways.
 * https://oeis.org/A025334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6187, u32 n <= 6187, u64 n <= 6187,
 *            u128 n <= 6187, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates A025334 – numbers that can be expressed as
 * the sum of three non‑zero squares in at least six unordered ways.
 *
 * The implementation enumerates all unordered triples (a,b,c)
 * with 1 ≤ a ≤ b ≤ c, computes s = a² + b² + c² up to a fixed
 * maximum value, counts how many triples give each s, and keeps
 * the first sufficiently many terms in a dynamically growing
 * array that is filled once lazily on the first call.
 *
 * The LIMIT is chosen large enough so that the cached array
 * contains at least the first 10 000 terms; it is 2 000 000.
 *
 * The algorithm runs in O(LIMIT^(3/2)) time and uses only
 * integer arithmetic (no floating point).  All casts from
 * malloc/calloc are explicit to satisfy both C11 and C++17. */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* --------------------------------------------------------------------- */
/* Configuration: limit large enough for many terms                      */
/* --------------------------------------------------------------------- */
#define LIMIT 2000000u          /* first 10 000 terms are below this value   */

/* --------------------------------------------------------------------- */
/* Static state: tables and term cache                                  */
/* --------------------------------------------------------------------- */
static uint8_t  *cnt = NULL;     /* number of unordered representations      */
static uint32_t *sqr = NULL;     /* a² for a up to sqrt(LIMIT)                */
static uint32_t *sqrt_floor = NULL;
static seq_t   *terms = NULL;    /* collected terms (strictly increasing)     */
static size_t   terms_cap = 0;   /* allocated capacity for terms array       */
static size_t   terms_count = 0; /* number of terms stored                   */

/* --------------------------------------------------------------------- */
/* Helper: grow the terms array                                          */
/* --------------------------------------------------------------------- */
static void grow_terms(void)
{
    size_t new_cap = terms_cap == 0 ? 8 : terms_cap * 2;
    seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp) abort();          /* unrecoverable in this context           */
    terms = tmp;
    terms_cap = new_cap;
}

/* --------------------------------------------------------------------- */
/* Build auxiliary tables and pre‑compute all terms                     */
/* --------------------------------------------------------------------- */
static void init_tables(void)
{
    /* Count array: initialise to zeros; capped at 7 */
    cnt = (uint8_t *)calloc(LIMIT + 1, sizeof(uint8_t));
    if (!cnt) abort();

    /* Square table up to sqrt(LIMIT) */
    uint32_t sqr_limit = 1;
    while ((sqr_limit + 1u) * (sqr_limit + 1u) <= LIMIT) ++sqr_limit;
    sqr = (uint32_t *)malloc((sqr_limit + 1) * sizeof(uint32_t));
    if (!sqr) abort();
    for (uint32_t i = 0; i <= sqr_limit; ++i) sqr[i] = i * i;

    /* floor(sqrt(x)) for 0 .. LIMIT  (used for quick bounds) */
    sqrt_floor = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!sqrt_floor) abort();
    uint32_t r = 0;
    for (uint32_t i = 0; i <= LIMIT; ++i) {
        while ((r + 1u) * (r + 1u) <= i) ++r;
        sqrt_floor[i] = r;
    }

    /* Enumerate all unordered non‑zero triples (a,b,c) */
    for (uint32_t a = 1; a <= sqrt_floor[LIMIT]; ++a) {
        uint32_t a2 = sqr[a];
        if (a2 > LIMIT) break;
        for (uint32_t b = a; b <= sqrt_floor[LIMIT]; ++b) {
            uint32_t b2 = sqr[b];
            uint32_t sum_ab = a2 + b2;
            if (sum_ab > LIMIT) break;
            for (uint32_t c = b; c <= sqrt_floor[LIMIT]; ++c) {
                uint32_t sum = sum_ab + sqr[c];
                if (sum > LIMIT) break;
                if (cnt[sum] < 7) ++cnt[sum];   /* we only need to count up to 6 */
            }
        }
    }

    /* Collect terms with at least 6 representations */
    for (uint32_t i = 1; i <= LIMIT; ++i) {
        if (cnt[i] >= 6) {
            if (terms_count == terms_cap) grow_terms();
            terms[terms_count++] = (seq_t)i;
        }
    }
}

/* --------------------------------------------------------------------- */
/* Public sequence function                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    static int initialized = 0;
    if (!initialized) {
        init_tables();
        initialized = 1;
    }

    /* Return 0 if n is outside the cached range (shouldn't happen). */
    if ((idx_t)(terms_count) < n) return 0;
    return terms[(size_t)(n - 1)];
}
