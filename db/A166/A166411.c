/*
 * A166411  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^11 = I.
 * https://oeis.org/A166411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*∑_{j=1..10} a(n-j) – 120*a(n-11) + r_n, where the
 *            inhomogeneous term r_n is r_0 = 1, r_1 … r_10 = 2, r_11 = 1, r_n
 *            = 0 for n > 11
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <assert.h>

seq_t a(idx_t n)
{
    /* the first 11 values given in the OEIS entry */
    static const seq_t init[11] = {
        (seq_t)1ULL,
        (seq_t)17ULL,
        (seq_t)272ULL,
        (seq_t)4352ULL,
        (seq_t)69632ULL,
        (seq_t)1114112ULL,
        (seq_t)17825792ULL,
        (seq_t)285212672ULL,
        (seq_t)4563402752ULL,
        (seq_t)73014444032ULL,
        (seq_t)1168231104512ULL
    };

    /* cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of entries currently stored  */
    idx_t need = n + 1;           /* indices 0 … n must be present   */

    if (cache_len < need) {
        /* grow cache to hold the required indices */
        seq_t *new_cache = (seq_t *)realloc(cache,
                                            (size_t)need * sizeof(seq_t));
        assert(new_cache && "memory allocation failed");
        cache = new_cache;

        /* initialize missing initial terms (indices 0 … 10) */
        if (cache_len <= 10) {
            for (idx_t i = cache_len; i <= 10 && i < need; ++i)
                cache[i] = init[i];
        }

        /* start computing new terms.
           If the cache already has more than 11 entries we start at cache_len;
           otherwise we start at 11 because indices 0…10 are known.          */
        idx_t start = (cache_len > 11) ? cache_len : 11;
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = 0;
            for (idx_t j = 1; j <= 10; ++j)
                sum += cache[i - j];

            seq_t val = (seq_t)15 * sum - (seq_t)120 * cache[i - 11];

            /* add the inhomogeneous term for i == 11 */
            if (i == 11)
                val += (seq_t)1;
            cache[i] = val;
        }

        cache_len = need;
    }

    return cache[n];
}
