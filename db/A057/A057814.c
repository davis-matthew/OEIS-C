/*
 * A057814  Number of partitions of an n-set into blocks of size > 4.
 * https://oeis.org/A057814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = Sum_{k=5..n} binomial(n-1,k-1) * a(n-k); a(0)=1,
 *            a(1..4)=0
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 20, u64 n <= 31, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#define MIN(a,b) ((a)<(b)?(a):(b))

seq_t a(idx_t n) {
    /* static cache of previously computed values */
    static seq_t *cache = NULL;     /* array of a[0..len-1] */
    static size_t cache_len = 0;    /* number of entries currently allocated */

    /* Resize cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_len = ((size_t)n) + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – abort */
            abort();
        }
        cache = tmp;
        /* initialize new entries */
        for (size_t i = cache_len; i < new_len; ++i) {
            cache[i] = 0;          /* default 0 for a[1..] */
        }
        if (cache_len == 0) {
            cache[0] = (seq_t)1;   /* a(0)=1 */
        }
        cache_len = new_len;

        /* Compute new values from the first missing index up to n */
        idx_t start = MIN((idx_t)5, (idx_t)cache_len);  /* values <5 are 0 */
        for (idx_t i = start; i <= n; ++i) {
            seq_t sum = 0;
            /* compute binomial(i-1,4) */
            seq_t bin = 1;
            for (idx_t j = 0; j < 4; ++j) {
                bin = bin * (seq_t)(i - 1 - j) / (seq_t)(j + 1);
            }
            /* iterate k=5..i */
            for (idx_t k = 5; k <= i; ++k) {
                /* bin = C(i-1, k-1) */
                sum += bin * cache[i - k];
                if (k < i) {
                    /* Update bin to C(i-1, k) for next k */
                    seq_t num = (seq_t)(i - k);   /* i-1 - (k-1) = i-k */
                    seq_t den = (seq_t)k;
                    bin = bin * num / den;
                }
            }
            cache[i] = sum;
        }
    }

    return cache[(size_t)n];
}
