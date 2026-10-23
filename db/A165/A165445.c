/*
 * A165445  Number of reduced words of length n in Coxeter group on 27
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^9 = I.
 * https://oeis.org/A165445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 27, a(2) = 702, a(3) = 18252, a(4) = 474552,
 *            a(5) = 12338352, a(6) = 320797152, a(7) = 8340725952, a(8) =
 *            216858874752, a(n) = 25*(a(n-1)+...+a(n-8)) - 325*a(n-9) + (n==9
 *            ? 1 : 0) (n ≥ 9) This recurrence comes from the rational
 *            generating function (t^9+2t^8+…+2t+1) / (325t^9-25t^...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* first nine terms: a(0) … a(8) */
static const seq_t init[9] = {
    1ULL,
    27ULL,
    702ULL,
    18252ULL,
    474552ULL,
    12338352ULL,
    320797152ULL,
    8340725952ULL,
    216858874752ULL
};

seq_t a(idx_t n)
{
    /* cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialise cache with the first nine terms */
    if (cache_len == 0) {
        cache_len = 9;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i)
            cache[i] = init[i];
    }

    /* trivial case: requested index already computed */
    if (n < cache_len)
        return cache[n];

    /* extend cache up to n */
    idx_t new_len = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)            /* allocation failure – unlikely for requested n */
        return cache[n];
    cache = tmp;

    /* compute terms from cache_len up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* sum of the previous eight terms */
        seq_t sum = 0;
        for (int k = 1; k <= 8; ++k)
            sum += cache[i - k];

        seq_t val = 25 * sum;                    /* 25*(a(i-1)+…+a(i-8)) */
        val -= 325 * cache[i - 9];                /* - 325*a(i-9)      */
        if (i == 9)                               /* add numerator term */
            val += 1;

        cache[i] = val;
    }

    cache_len = new_len;
    return cache[n];
}
