/*
 * A079994  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={0,1}.
 * https://oeis.org/A079994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 30, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A079994 (OEIS) – Number of permutations satisfying
    -3 ≤ p(i)-i ≤ 3 and p(i)-i ∉ {0,1}.
    Recurrence (coefficients are taken for all indices ≥ 20):
        a(n) = 2*a(n-2) + 2*a(n-3) + 4*a(n-4) + 9*a(n-5) +10*a(n-6)
             - 3*a(n-7) - 9*a(n-8) - a(n-10) -12*a(n-11) -11*a(n-12)
             + a(n-13) + 4*a(n-14) - a(n-15) + 2*a(n-18) - a(n-20)
    Initial values (n = 0 … 19):
        1, 0, 0, 1, 3, 9, 13, 25, 59, 147,
        328, 690, 1478, 3285, 7357, 16249, 35561, 77974,
        171891, 379401
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;          /* memoised values */
    static idx_t cache_len = 0;          /* number of values stored */
    static const seq_t base20[20] = {
        1ULL, 0ULL, 0ULL, 1ULL, 3ULL, 9ULL, 13ULL, 25ULL, 59ULL, 147ULL,
        328ULL, 690ULL, 1478ULL, 3285ULL, 7357ULL, 16249ULL, 35561ULL,
        77974ULL, 171891ULL, 379401ULL
    };

    /* Initialise cache with the first 20 values the first time a() is called */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 20);
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = base20[i];
        cache_len = 20;
    }

    /* If requested value is already known, return it */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to hold the requested index */
    size_t new_len = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache)
        abort();      /* out of memory */
    cache = new_cache;

    /* Compute all missing terms in order; all references are to lower indices */
    for (idx_t i = cache_len; i < new_len; ++i) {
        seq_t pos = 0, neg = 0;

        /* Positive contributions */
        pos += cache[i-2] * 2;
        pos += cache[i-3] * 2;
        pos += cache[i-4] * 4;
        pos += cache[i-5] * 9;
        pos += cache[i-6] * 10;
        pos += cache[i-13] * 1;
        pos += cache[i-14] * 4;
        pos += cache[i-18] * 2;

        /* Negative contributions */
        neg += cache[i-7] * 3;
        neg += cache[i-8] * 9;
        neg += cache[i-10] * 1;
        neg += cache[i-11] * 12;
        neg += cache[i-12] * 11;
        neg += cache[i-15] * 1;
        neg += cache[i-20] * 1;

        cache[i] = pos - neg;
    }

    cache_len = new_len;
    return cache[n];
}
