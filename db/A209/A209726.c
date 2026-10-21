/*
 * A209726  1/4 the number of (n+1) X 8 0..2 arrays with every 2 X 2 subblock
 *          having distinct clockwise edge differences.
 * https://oeis.org/A209726
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*2^(n/2-1) + 14 for even n, 2^((n+1)/2) + 14 for odd n
 *            (Recurrence: a(n) = a(n-1) + 2*a(n-2) - 2*a(n-3), initial values
 *            a(1)=16, a(2)=17, a(3)=18)
 * Range:     u8 n <= 14, u16 n <= 30, u32 n <= 62, u64 n <= 126,
 *            u128 n <= 210, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; if n == 0 return 0. */
    if (n == 0) return (seq_t)0;

    if ((n & 1) == 0) {            /* n even */
        idx_t exp = n / 2 - 1;     /* exp >= 0 because n >= 2 */
        seq_t v   = pow2(exp);
        return 3 * v + (seq_t)14;
    } else {                      /* n odd */
        idx_t exp = (n + 1) / 2;   /* exp >= 1 because n >= 1 */
        seq_t v   = pow2(exp);
        return v + (seq_t)14;
    }
}
