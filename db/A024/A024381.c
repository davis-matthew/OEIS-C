/*
 * A024381  a(n) = sum of squares of first n positive integers congruent to 1
 *          mod 4.
 * https://oeis.org/A024381
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^n (4k-3)^2 = (8 n(n+1)(2n+1)-36 n(n+1)+27 n)/3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 650, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(1) is 1 by definition; for n == 0 return 0 (below offset) */
    if (n == 0) {
        return (seq_t)0;
    }

    seq_t N = (seq_t)n;

    /* Term1 = 8 * N * (N+1) * (2*N+1) */
    seq_t term1 = (seq_t)8 * N * (N + (seq_t)1) * ( (seq_t)2 * N + (seq_t)1 );

    /* Term2 = 36 * N * (N+1) */
    seq_t term2 = (seq_t)36 * N * (N + (seq_t)1);

    /* Term3 = 27 * N */
    seq_t term3 = (seq_t)27 * N;

    /* Numerator = Term1 - Term2 + Term3 */
    seq_t num = term1 - term2 + term3;

    /* Result = Numerator / 3 */
    return num / (seq_t)3;
}
