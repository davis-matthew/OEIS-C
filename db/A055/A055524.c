/*
 * A055524  Longest other side of a Pythagorean triangle with n as length of
 *          one of the three sides (in fact n is a leg and a(n) the
 *          hypotenuse).
 * https://oeis.org/A055524
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2+6-(n^2-2)*(-1)^n)/8 Simplified: a(n) = (n^2+4)/4
 *            for n even, a(n) = (n^2+1)/2 for n odd (n ≥ 3)
 * Range:     u8 n <= 15, u16 n <= 361, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* values below the offset are arbitrary */

    seq_t n2 = (seq_t)n * (seq_t)n;

    if ((n & 1) == 0)             /* n even */
        return (n2 + 4) / 4;
    else                          /* n odd  */
        return (n2 + 1) / 2;
}
