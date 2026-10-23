/*
 * A169402  Number of reduced words of length n in Coxeter group on 5
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^32 = I.
 * https://oeis.org/A169402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=5; * For 2<=n<=31: a(n)=4*a(n-1); *
 *            a(32)=4*a(31)-10; * For n>=33: a(n)=3*Σ_{i=1}^{31}
 *            a(n-i)-6*a(n-32); * (Follows from
 *            (1+2∑_{k=1}^{31}x^k+x^32)/(1-3∑_{k=1}^{31}x^k+6x^32)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        cache = tmp;

        for (idx_t k = cache_size; k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = 1;
            } else if (k == 1) {
                val = 5;
            } else if (k <= 31) {
                val = cache[k - 1] * 4;
            } else if (k == 32) {
                val = cache[31] * 4 - 10;
            } else {
                seq_t sum = 0;
                for (idx_t i = 1; i <= 31; ++i) {
                    sum += cache[k - i];
                }
                val = sum * 3 - cache[k - 32] * 6;
            }
            cache[k] = val;
        }
        cache_size = new_size;
    }
    return cache[n];
}
