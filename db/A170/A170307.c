/*
 * A170307  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(n)=10*9^(n-1) for 1≤n≤41; for n≥42:
 *            a(n)=8*∑_{i=1}^{41} a(n-i) - 36*a(n-42)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#define MAX_N 10001                     /* enough for all tests up to n≈10000 */
static seq_t cache[MAX_N];
static idx_t cache_size = 0;            /* number of terms computed (cache[0..cache_size-1]) */

seq_t a(idx_t n)
{
    if (n >= cache_size) {              /* compute terms up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)10;
            } else if (i < 42) {            /* 2 … 41 */
                cache[i] = cache[i-1] * (seq_t)9;
            } else if (i == 42) {
                seq_t sum = 0;
                for (idx_t j = 1; j <= 41; ++j) {
                    sum += cache[i-j];
                }
                cache[i] = (seq_t)1 + sum * (seq_t)8 - cache[0] * (seq_t)36;
            } else {                        /* i ≥ 43 */
                seq_t sum = 0;
                for (idx_t j = 1; j <= 41; ++j) {
                    sum += cache[i-j];
                }
                cache[i] = sum * (seq_t)8 - cache[i-42] * (seq_t)36;
            }
        }
        cache_size = n + 1;
    }
    return cache[n];
}
