/*
 * A080448  a(1) = 4, a(n) = smallest (nontrivial) multiple of a(n-1)
 *          containing n digits, a(n) not equal to 10*a(n-1). Also a(n) is not
 *          divisible by 10.
 * https://oeis.org/A080448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A080448: a(1)=4; for n≥2
 * a(n)=smallest non‑trivial multiple of a(n‑1)
 * that has exactly n decimal digits, is not 10*a(n‑1), and not divisible by 10. */

#include <stdlib.h> /* malloc, realloc */

static seq_t *cache = NULL;      /* memoised values */
static idx_t cache_size = 0;     /* current capacity of cache */
static idx_t computed_up_to = 0; /* highest index computed */
static seq_t pow10_nMinus1 = 0;  /* 10^(computed_up_to)  (next low bound) */

seq_t a(idx_t n)
{
    /* handle the first term */
    if (n == 1U)
        return (seq_t)4;

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_size = 4;                 /* initial capacity */
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[1] = (seq_t)4;            /* a(1) */
        computed_up_to = 1U;
        pow10_nMinus1 = (seq_t)10;      /* low bound for n=2 */
    }

    /* return cached value if already available */
    if (n <= computed_up_to)
        return cache[n];

    /* enlarge cache if necessary */
    if (n >= cache_size) {
        size_t new_cap = cache_size * 2U;
        if (new_cap < (size_t)(n + 1U))
            new_cap = (size_t)(n + 1U);
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        cache_size = new_cap;
    }

    /* compute all required terms up to n */
    for (idx_t i = computed_up_to + 1U; i <= n; ++i) {
        seq_t low  = pow10_nMinus1;               /* 10^(i‑1)      */
        seq_t high = low * (seq_t)10;             /* 10^i          */
        seq_t prev = cache[i - 1U];               /* a(i‑1)         */
        seq_t result = 0;

        /* search for the smallest suitable multiple of prev */
        for (idx_t k = 2U; ; ++k) {
            seq_t prod = prev * (seq_t)k;

            if (prod >= high)        /* too many digits → impossible      */
                break;
            if (k == 10U)
                continue;              /* skip the forbidden multiple      */
            if (prod % (seq_t)10 == 0)
                continue;              /* divisible by 10 – reject          */

            if (prod >= low) {        /* has exactly i digits            */
                result = prod;
                break;
            }
        }

        cache[i] = result;
        pow10_nMinus1 *= (seq_t)10;   /* advance low bound for next i     */
    }

    computed_up_to = n;
    return cache[n];
}
