/*
 * A171835  Partial sums of numbers congruent to {3, 4, 5, 6} mod 8 (A047425).
 * https://oeis.org/A171835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)/2 + 2*n + 8*floor(n/4)*(floor(n/4)-1) +
 *            4*floor(n/4)*(n%4)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;
    seq_t sum_i = n_seq * (n_seq + 1) / 2;   /* n*(n+1)/2 */
    seq_t two_n = 2 * n_seq;                 /* 2*n */

    /* m = floor(n/4), r = n mod 4 */
    idx_t m_idx = n / 4;
    idx_t r_idx = n % 4;
    seq_t m = (seq_t)m_idx;
    seq_t r = (seq_t)r_idx;

    seq_t term1 = (m == 0) ? (seq_t)0 : 8 * m * (m - 1);   /* 8*m*(m-1) safely */
    seq_t term2 = 4 * m * r;                                /* 4*m*r */

    return sum_i + two_n + term1 + term2;
}
