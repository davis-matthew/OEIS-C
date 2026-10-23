/*
 * A138146  Palindromes with 2n-1 digits formed from the reflected decimal
 *          expansion of the concatenation of 1, 1, 1 and infinite 0's.
 * https://oeis.org/A138146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 111 + 111*100^(n-2) (for n > 3) a(1) = 1, a(2) = 111,
 *            a(3) = 11111 (Using the provided pow10 utility for efficiency.)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 12,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1)   return (seq_t)1;
    if (n == 2)   return (seq_t)111;
    if (n == 3)   return (seq_t)11111;
    if (n == 0)   return (seq_t)0;          /* safeguard for unexpected input */

    /* For n > 3: a(n) = 111 * (1 + 100^(n-2)) = 111 * (1 + 10^(2*(n-2))) */
    idx_t exp = 2 * (n - 2);               /* exponent of 10 */
    seq_t pw   = pow10(exp);               /* 10^exp = 100^(n-2) */
    seq_t tmp  = pw + (seq_t)1;            /* (100^(n-2) + 1)   */
    return tmp * (seq_t)111;              /* 111 * above   */
}
