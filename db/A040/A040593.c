/*
 * A040593  Continued fraction for sqrt(618).
 * https://oeis.org/A040593
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, less
 * Formula:   a(n) = 1 if n==1 or no factor 2 or 3; * a(n) = 6 if 2|n and 3∤n;
 *            * a(n) = 8 if 3|n and 2∤n; * a(n) = 48 if 2|n and 3|n; * a(0) =
 *            24 (special case)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 24;

    seq_t r = 1;
    if ((n % 2) == 0)
        r *= 6;
    if ((n % 3) == 0)
        r *= 8;
    return r;
}
