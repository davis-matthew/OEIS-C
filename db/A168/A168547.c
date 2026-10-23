/*
 * A168547  a(n) = 1 - 2*n^2 + 4*n*(1 + 2*n^2)/3.
 * https://oeis.org/A168547
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - 2*n^2 + 4*n*(1 + 2*n^2)/3
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 1171, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Cast index to seq_t for computations */
    seq_t nt = (seq_t)n;

    /* n^2 */
    seq_t n2 = nt * nt;

    /* 2*n^2 */
    seq_t two_n2 = (seq_t)2 * n2;

    /* 1 + 2*n^2 */
    seq_t one_plus_two_n2 = ((seq_t)1) + two_n2;

    /* 4*n*(1 + 2*n^2) */
    seq_t four_n_times = (seq_t)4 * nt * one_plus_two_n2;

    /* Divide by 3 (division exact) */
    seq_t div_by_three = four_n_times / (seq_t)3;

    /* a(n) = 1 - 2*n^2 + (4*n*(1 + 2*n^2))/3 */
    seq_t result = ((seq_t)1) - two_n2 + div_by_three;

    return result;
}
