/*
 * A019560  Coordination sequence for C_4 lattice.
 * https://oeis.org/A019560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, for n>0: a(n) = (32/3) * n * (1 + 2*n^2)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 586, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t nn = (seq_t)n;
    /* Compute n * (1 + 2*n^2) */
    seq_t term = nn * ( (seq_t)1 + (seq_t)2 * nn * nn );
    /* Divide by 3 (divisible for all n>0) */
    term /= (seq_t)3;
    /* Multiply by 32 */
    term *= (seq_t)32;
    return term;
}
