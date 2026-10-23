/*
 * A136281  Number of graphs on n labeled nodes with degree at most 2.
 * https://oeis.org/A136281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n-1)*a(n-1) - (n-1)^2*a(n-2) + (n-2)*(n-1)*a(n-3) -
 *            ((n-3)*(n-2)*(n-1)/2)*a(n-4) Initial values: a(0)=1, a(1)=1,
 *            a(2)=2, a(3)=8
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;

    if (n >= cap) {
        seq_t *tmp;
#ifdef __cplusplus
        tmp = reinterpret_cast<seq_t *>(
            realloc(cache, (size_t)(n + 1) * sizeof(seq_t)));
#else
        tmp = (seq_t *)realloc(cache,
            (size_t)(n + 1) * sizeof(seq_t));
#endif
        cache = tmp;
        for (idx_t i = cap; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)1;
            } else if (i == 2) {
                cache[i] = (seq_t)2;
            } else if (i == 3) {
                cache[i] = (seq_t)8;
            } else {
                seq_t term1 = (seq_t)(2 * i - 1) * cache[i - 1];
                seq_t term2 = (seq_t)(i - 1) * (seq_t)(i - 1) * cache[i - 2];
                seq_t term3 = (seq_t)(i - 2) * (seq_t)(i - 1) * cache[i - 3];
                seq_t term4 = cache[i - 4];
                term4 *= (i - 1);
                term4 *= (i - 2);
                term4 *= (i - 3);
                term4 /= (seq_t)2;
                cache[i] = term1 - term2 + term3 - term4;
            }
        }
        cap = n + 1;
    }
    return cache[n];
}
