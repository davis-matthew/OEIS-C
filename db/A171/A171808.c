/*
 * A171808  Number of 5 X 5 permutation matrices such that the n-th matrix
 *          power is the identity matrix and the determinant is +1.
 * https://oeis.org/A171808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn, full
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 5, u128 n <= 5,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: count even permutations of S5 (5×5 permutation matrices with det +1) whose order is n.
   That is, a(n) = 0 for n > 5, and for n = 1, 2, 3, 5 the values are 1, 15, 20, 24 respectively. */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;   /* identity */
    if (n == 2) return (seq_t)15;  /* double transpositions */
    if (n == 3) return (seq_t)20;  /* 3‑cycles */
    if (n == 5) return (seq_t)24;  /* 5‑cycles */
    return (seq_t)0;
}
