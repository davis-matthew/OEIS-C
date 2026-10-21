/*
 * A231087  Number of perfect matchings in graph C_3 x C_{2n}.
 * https://oeis.org/A231087
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) – a(n-2) – 12 with a(2) = 50, a(3) = 224 for all
 *            n ≥ 4 (offset 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* values below the offset are undefined – return 0 */
    if (n < 2)
        return 0;

    /* base cases */
    if (n == 2)
        return (seq_t)50;
    if (n == 3)
        return (seq_t)224;

    /* iterate the recurrence */
    seq_t a_minus2 = (seq_t)50;   /* a(2) */
    seq_t a_minus1 = (seq_t)224;  /* a(3) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i)
    {
        cur = (seq_t)5 * a_minus1 - a_minus2 - (seq_t)12;
        a_minus2 = a_minus1;
        a_minus1 = cur;
    }
    return a_minus1;
}
