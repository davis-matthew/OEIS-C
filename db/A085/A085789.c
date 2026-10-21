/*
 * A085789  Partial sums of n 3-spaced triangular numbers beginning with t(2),
 *          e.g., a(2) = t(2) + t(5) = 3 + 15 = 18.
 * https://oeis.org/A085789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^2*(n+1)/2
 * Range:     u8 n <= 5, u16 n <= 34, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) /* offset is 1; n=0 is undefined, return 0 for safety */
        return 0;

    seq_t nn = (seq_t)n;

    /* Compute n^2*(n+1) first and divide by 2 before multiplying by 3
       to avoid intermediate overflow and ensure exact division. */
    seq_t term = nn * nn;
    term = term * (nn + 1);
    term /= 2;
    term = term * 3;

    return term;
}
