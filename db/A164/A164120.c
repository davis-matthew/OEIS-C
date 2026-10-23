/*
 * A164120  Partial sums of A162396.
 * https://oeis.org/A164120
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*2^((n-1)/2) - 7 for odd n, a(n) = 7*2^(n/2) - 7 for
 *            even n. Implements the recurrence a(n) = 2*a(n-2) + 7 with
 *            a(1)=5, a(2)=7
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 58, u64 n <= 122,
 *            u128 n <= 250, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* out of OEIS range, not used */
    if ((n & 1) == 0) {            /* even n */
        idx_t k = n >> 1;          /* k = n/2 */
        seq_t p = pow2(k);         /* p = 2^k */
        return (seq_t)7 * p - (seq_t)7;
    } else {                       /* odd n */
        idx_t k = (n - 1) >> 1;    /* k = (n-1)/2 */
        seq_t p = pow2(k);         /* p = 2^k */
        return (seq_t)12 * p - (seq_t)7;
    }
}
