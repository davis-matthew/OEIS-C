/*
 * A091760  a(n) = 0^n + 3*((n+2)^n/(n+3) - (-1)^n/(n+3)).
 * https://oeis.org/A091760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 * ((n+2)^n - (-1)^n) / (n+3)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    /* Compute (n+2)^n */
    seq_t pow = big_pow((seq_t)(n + 2), (seq_t)n);
    /* Adjust for (-1)^n */
    if ((n & 1) == 0) /* n even: subtract 1 */
        pow -= 1;
    else              /* n odd : add 1 */
        pow += 1;
    /* Multiply by 3 and divide by n+3 */
    pow *= 3;
    seq_t result = pow / (n + 3);
    return result;
}
