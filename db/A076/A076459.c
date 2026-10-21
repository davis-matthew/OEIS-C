/*
 * A076459  Sum of numbers that can be written as t*n + u*(n+1) for
 *          nonnegative integers t,u in exactly n ways.
 * https://oeis.org/A076459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(2*n^3+2*n^2-2*n-1)/2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 73, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle offset: sequence defined for n >= 1 */
    if (n == 0)
        return 0;

    seq_t n_seq = (seq_t)n;
    seq_t n_plus1 = n_seq + 1;

    seq_t n_sq = n_seq * n_seq;
    seq_t n_cu = n_sq * n_seq;

    seq_t inner = 2 * n_cu + 2 * n_sq - 2 * n_seq - (seq_t)1;

    seq_t result = n_seq * n_plus1 * inner / 2;
    return result;
}
