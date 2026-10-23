/*
 * A014707  a(4n) = 0, a(4n+2) = 1, a(2n+1) = a(n).
 * https://oeis.org/A014707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(4n) = 0, a(4n+2) = 1, a(2n+1) = a(n) (a(0)=0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Remove trailing 1 bits (odd part) by shifting right. */
    while ((n & 1) != 0)
        n >>= 1;

    /* Now n is even (including zero).  The value depends only on n mod 4. */
    if ((n & 3) == 0)   /* n % 4 == 0, i.e. n = 4k  */
        return (seq_t)0;
    else                /* n % 4 == 2, i.e. n = 4k+2  */
        return (seq_t)1;
}
