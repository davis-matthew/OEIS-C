/*
 * A101555  Convolution of A010060 and A000244.
 * https://oeis.org/A101555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} A010060(n-k) * 3^k
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   parity_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = 3 * a(n-1) + A010060(n),  a(0)=0 */
/*       A010060(n) = 0 if popcount(n) is even, 1 otherwise (Thue–Morse) */

#include <stdint.h>
#include "../utils/parity_u64.h"

seq_t a(idx_t n)
{
    seq_t val = 0;                     /* a(0) = 0 */
    for (idx_t i = 1; i <= n; ++i) {   /* iterate from 1 to n */
        val = 3 * val + (seq_t)parity_u64(i);  /* a(i) = 3*a(i-1) + A010060(i) */
    }
    return val;
}
