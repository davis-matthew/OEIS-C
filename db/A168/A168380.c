/*
 * A168380  Row sums of A168281.
 * https://oeis.org/A168380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(3 + 2*n^2 + 4*n - 3*(-1)^n)/12
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1504, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* offset starts at 1, return 0 if called with 0 */

    seq_t nn = (seq_t)n;
    /* term = 2*n^2 + 4*n + (n odd ? 6 : 0) */
    seq_t term = 2 * nn * nn + 4 * nn + ((n & 1) ? 6 : 0);
    seq_t num  = (nn + 1) * term;
    return num / 12;
}
