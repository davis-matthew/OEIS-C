/*
 * A145066  Partial sums of A002522, starting at n=1.
 * https://oeis.org/A145066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(2*n+1)/6 + n
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1409, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset 1 – value for n<1 is undefined */

    seq_t nn = (seq_t)n;          /* convert to seq_t once for type safety */

    /* compute n*(n+1)*(2*n+1)/6 + n, all in seq_t arithmetic */
    seq_t result = nn * (nn + 1) * (2 * nn + 1) / 6 + nn;

    return result;
}
