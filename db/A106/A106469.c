/*
 * A106469  Expansion of (1 + x^2)*(1 + 2*x)/(1 - x^2).
 * https://oeis.org/A106469
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; a(1) = 2; a(2) = 2; a(3) = 4; For n > 3, a(n) =
 *            a(n-2). Equivalently: a(n) = 2 if n is even and n > 0; a(n) = 4
 *            if n is odd and n > 1; (a(1) remains 2)
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;          /* a(0) = 1 */
    if ((n & 1) == 0)      /* n even, n > 0 */
        return 2;          /* a(n) = 2 for even n >= 2 */
    if (n == 1)
        return 2;          /* a(1) = 2 */
    return 4;              /* a(n) = 4 for odd n >= 3 */
}
