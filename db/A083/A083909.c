/*
 * A083909  Numbers of the form 10^(m-k)*(10^(m+k+1)-10^k), m, k >= 0.
 * https://oeis.org/A083909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 10^(2*n-1) - 10^(n-1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* n must be >= 1; return 0 for n==0 for safety. */
    if (n == 0)
        return (seq_t)0;

    idx_t e1 = 2*n - 1;    /* exponent for 10^(2*n-1) */
    idx_t e2 = n - 1;      /* exponent for 10^(n-1)  */

    seq_t p1 = pow10(e1);
    seq_t p2 = pow10(e2);
    return p1 - p2;
}
