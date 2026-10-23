/*
 * A123657  a(n) = 1 + n^4 + n^6 + n^9.
 * https://oeis.org/A123657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + n^4 + n^6 + n^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 11, u64 n <= 138, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t ns  = (seq_t)n;      /* n as seq_t */
    seq_t n2  = ns * ns;       /* n^2 */
    seq_t n4  = n2 * n2;       /* n^4 */
    seq_t n6  = n4 * n2;       /* n^6 */
    seq_t n8  = n4 * n4;       /* n^8 */
    seq_t n9  = n8 * ns;       /* n^9 */

    return (seq_t)1 + n4 + n6 + n9;
}
