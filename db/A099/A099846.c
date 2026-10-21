/*
 * A099846  An Alexander sequence for the knot 8_5.
 * https://oeis.org/A099846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-2) + 5*a(n-3) - 4*a(n-4) + 3*a(n-5) -
 *            a(n-6) a(0)=1, a(1)=3, a(2)=5, a(3)=8, a(4)=15, a(5)=29
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* initialize cache with first six values */
    if (cache_len == 0) {
        cache_len = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return (seq_t)0;          /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)29;
    }

    /* already computed? */
    if (n < cache_len)
        return cache[(size_t)n];

    /* grow cache to accommodate index n */
    idx_t old_len = cache_len;
    idx_t new_len = n + 1;
    if (new_len > cache_len) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp) return (seq_t)0;          /* allocation failure */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute new terms up to n */
    for (idx_t i = old_len; i <= n; ++i) {
        seq_t val = cache[i - 1] * (seq_t)3;
        val -= cache[i - 2] * (seq_t)4;
        val += cache[i - 3] * (seq_t)5;
        val -= cache[i - 4] * (seq_t)4;
        val += cache[i - 5] * (seq_t)3;
        val -= cache[i - 6];
        cache[i] = val;
    }

    return cache[(size_t)n];
}
