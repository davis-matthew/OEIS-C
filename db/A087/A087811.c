/*
 * A087811  Numbers k such that ceiling(sqrt(k)) divides k.
 * https://oeis.org/A087811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = ((n + n mod 2)*(n + 2 - n mod 2))/4
 * Range:     u8 n <= 30, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset start 1; for n==0 produce 0 though unused */
    if (n == 0)
        return 0;

    seq_t m = (seq_t)(n + 2) / 2; /* m = floor((n+2)/2) */

    if (n & 1)   /* odd n -> m^2   */
        return m * m;
    else         /* even n -> m^2 - m */
        return m * m - m;
}
