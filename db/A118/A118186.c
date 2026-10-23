/*
 * A118186  Row sums of triangle A118185: a(n) = Sum_{k=0..n} 4^(k*(n-k)) for
 *          n>=0.
 * https://oeis.org/A118186
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} 4^(k*(n-k))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate an array pow4[0..n] to hold 4^i */
    size_t len = (size_t)(n + 1);
    seq_t *pow4 = (seq_t *)malloc(len * sizeof(seq_t));
    if (!pow4)
        return 0;  /* allocation error; unlikely with small n */

    pow4[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        pow4[i] = pow4[i - 1] * (seq_t)4;

    seq_t term = (seq_t)1;          /* 4^(0*(n-0)) */
    seq_t sum  = term;              /* include k = 0 */

    for (idx_t k = 1; k <= n; ++k) {
        long long diff = (long long)n - 2 * (long long)k + 1;
        if (diff >= 0) {
            term = term * pow4[(idx_t)diff];
        } else {
            term = term / pow4[(idx_t)(-diff)];
        }
        sum += term;
    }

    free(pow4);
    return sum;
}
