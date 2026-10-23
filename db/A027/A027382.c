/*
 * A027382  a(n) = n^4 - 6*n^3 + 12*n^2 - 4*n + 1.
 * https://oeis.org/A027382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 6*n^3 + 12*n^2 - 4*n + 1
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    /* Compute n^4 + 12*n^2 + 1 first to avoid underflow when subtracting. */
    seq_t result = n4;
    result += 12 * n2;
    result += 1;
    result -= 6 * n3;
    result -= 4 * nn;

    return result;
}
