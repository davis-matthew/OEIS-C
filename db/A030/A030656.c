/*
 * A030656  Pair up the numbers.
 * https://oeis.org/A030656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = Concatenation of 2*n and 2*n+1 Equivalent to a(n) = (2*n)
 *            * 10^{digits(2*n+1)} + (2*n+1)
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m1 = (seq_t)n * 2;   /* 2*n */
    seq_t m2 = m1 + 1;         /* 2*n+1 */

    /* Count decimal digits of m2 */
    int d = 0;
    seq_t tmp = m2;
    while (tmp > 0) {
        tmp /= 10;
        d++;
    }

    /* Compute 10^d */
    seq_t pow10 = 1;
    for (int i = 0; i < d; ++i)
        pow10 *= 10;

    /* Concatenate */
    return m1 * pow10 + m2;
}
