/*
 * A208665  Numbers that match odd ternary polynomials; see Comments.
 * https://oeis.org/A208665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = 3 * ( \sum_{i=0}^{\lfloor\log_3 n\rfloor} (n \text{ mod }
 *            3^{i+1}/3^i) * 9^i )
 * Range:     u8 n <= 11, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* That is, write n in base 3 with digits d_i, then a(n)=3 * \sum d_i * 9^i. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of offset; behaviour undefined anyway   */
    seq_t value = 0;
    seq_t pow9   = 1;
    idx_t tmp    = n;

    while (tmp > 0)
    {
        idx_t digit = tmp % 3;             /* digit is 0,1,2 in base 3 */
        value += (seq_t)digit * pow9;
        pow9  *= 9;
        tmp  /= 3;
    }
    return (seq_t)3 * value;
}
