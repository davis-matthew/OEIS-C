/*
 * A063228  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          67 ).
 * https://oeis.org/A063228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 5            for n = 1
 *             11*n - 6        for even n >= 2
 *             11*n - 5        for odd  n >= 3
 *
 * This follows from the recurrence a(n) = a(n-1) + (12 if n odd else 10) for n >= 3,
 * with a(1) = 5, a(2) = 16. The explicit formulas above give the same values
 * for all n >= 1.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)5;

    /* Check parity with bitwise AND for efficiency */
    if ((n & (idx_t)1) == 0) /* even n */
        return (seq_t)11 * (seq_t)n - (seq_t)6;
    else                    /* odd n */
        return (seq_t)11 * (seq_t)n - (seq_t)5;
}
