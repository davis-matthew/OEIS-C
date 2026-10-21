/*
 * A062346  Consider 2n tennis players; a(n) is the number of matches needed
 *          to let every possible pair play each other.
 * https://oeis.org/A062346
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n^2-3*n+1, 2) = n*(4*n^3-12*n^2+11*n-3)/2
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 215, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* sequence is defined for n ≥ 2 */

    seq_t nn = (seq_t)n;
    /* M = 2*n^2 - 3*n + 1, all computations are in seq_t to avoid overflow */
    seq_t M = (seq_t)2 * nn * nn - (seq_t)3 * nn + 1;

    /* a(n) = M*(M-1)/2  ; M*(M-1) is always even */
    seq_t prod = M * (M - 1);
    return prod / 2;
}
