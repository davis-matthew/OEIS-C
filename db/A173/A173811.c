/*
 * A173811  a(n) = (8*10^n - 53)/9 for n > 0.
 * https://oeis.org/A173811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*10^n - 53)/9 for n > 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for the sequence, but harmless */
        return (seq_t)0;

    seq_t repunit = (seq_t)1;  /* repunit(n) = (10^n - 1)/9 = 111...1 (n digits) */

    for (idx_t i = 2; i <= n; ++i)
    {
        repunit = repunit * (seq_t)10 + (seq_t)1;  /* repunit = 10*repunit + 1 */
    }

    seq_t result = repunit * (seq_t)8 - (seq_t)5;    /* a(n) = 8*repunit - 5 */
    return result;
}
