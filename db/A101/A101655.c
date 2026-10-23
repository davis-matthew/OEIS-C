/*
 * A101655  a(n)=Product{k=0..n, 1+9^A010060(k)}/2.
 * https://oeis.org/A101655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product{k=0..n, 1+9^A010060(k)}/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   count_odd, pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation exploits that A010060(k) is the Thue-Morse sequence
   (parity of popcount(k)), so each factor is either 2 or 10.  The product
   simplifies to 2^{count_even-1} * 10^{count_odd}.  Here count_even and
   count_odd are the number of integers in [0..n] with even/odd popcount. */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow10.h"
#include "../utils/count_odd.h"

seq_t a(idx_t n)
{
    idx_t odd   = count_odd(n);
    idx_t even  = (n + 1) - odd;      /* total numbers minus odd ones */

    /* a(n) = 2^{even-1} * 10^{odd}  (even >= 1 for all n >= 0) */
    seq_t result = pow2(even - 1);
    result *= pow10(odd);
    return result;
}
