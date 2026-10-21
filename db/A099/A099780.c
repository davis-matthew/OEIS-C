/*
 * A099780  a(n) = Sum_{k=0..floor(n/3)} C(n-k, 2*k) * 2^k*3^(n-3*k).
 * https://oeis.org/A099780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3), a(0)=1, a(1)=3, a(2)=9
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* cached values */
    static idx_t cache_len = 0;     /* number of elements stored (>=0) */

    if (n >= cache_len) {           /* need to compute up to n */
        idx_t new_cap = (cache_len == 0) ? (n + 1) : cache_len * 2;
        while (new_cap <= n) new_cap <<= 1;

        /* realloc must be casted in C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;        /* allocation failed */
        cache = tmp;

        /* compute missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0) {
                val = 1;
            } else if (i == 1) {
                val = 3;
            } else if (i == 2) {
                val = 9;
            } else {
                /* a(i) = 6*a(i-1) - 9*a(i-2) + 2*a(i-3) */
                seq_t temp = 6 * cache[i - 1] + 2 * cache[i - 3];
                temp -= 9 * cache[i - 2];
                val = temp;
            }
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
