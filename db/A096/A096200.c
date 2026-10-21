/*
 * A096200  a(n) = n*(n-1)*(n-2)*(3*n-2)/6.
 * https://oeis.org/A096200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)*(n-2)*(3*n-2)/6
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special handling for n < 3 to avoid negative intermediate in unsigned arithmetic */
    if (n < 3)
        return 0;

    seq_t res = (seq_t)n;
    res *= (seq_t)(n - 1);
    res *= (seq_t)(n - 2);
    res *= ((seq_t)3 * n - 2);
    res /= 6;
    return res;
}
