/*
 * A226359  Number of ordered triples (i,j,k) with |i|, |j|, |k|, |i*j*k| <=
 *          n.
 * https://oeis.org/A226359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (i,j,k) of integers with
 *            |i|,|j|,|k|,|i*j*k| <= n. The expression below computes this
 *            directly using the derived summation: a(n) = (2n+1)² + 2n(4n+1)
 *            + 8∑_{a=1}^{n}∑_{b=1}^{⌊n/a⌋}⌊n/(ab)⌋ where the double sum is
 *            over positive a,b. The computation is performed in O...
 * Range:     u8 n <= 3, u16 n <= 69, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* (2n+1)²  */
    seq_t two_n_plus_one = (seq_t)(2 * n + 1);
    seq_t base = two_n_plus_one * two_n_plus_one;

    /* 2n(4n+1)  - extra k values for |i*j| = 0  */
    seq_t zero_pairs = (seq_t)(4 * n + 1);           /* number of (i,j) with i=0 or j=0 */
    seq_t extra_zero = (seq_t)2 * n_seq * zero_pairs;

    /* 8∑_{a=1}^{n}∑_{b=1}^{⌊n/a⌋}⌊n/(ab)⌋  - extra k values for 0<|i*j|≤n */
    seq_t sum_floor = 0;
    for (idx_t a = 1; a <= n; ++a) {
        idx_t b_max = n / a;                          /* limit for which ab ≤ n */
        for (idx_t b = 1; b <= b_max; ++b) {
            sum_floor += n_seq / ((seq_t)a * (seq_t)b);
        }
    }
    seq_t extra_sum = (seq_t)8 * sum_floor;

    return base + extra_zero + extra_sum;
}
