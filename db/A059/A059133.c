/*
 * A059133  A hierarchical sequence (S(W2{2}c) - see A059126).
 * https://oeis.org/A059133
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 20*2^n - 6*n - 16, a(0)=4, a(1)=18, …
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A059133.
   The computation uses only integer arithmetic and works for all
   integer sequence types supported by the build. */
seq_t a(idx_t n)
{
    /* compute 2^n as a left shift of 1 */
    seq_t pow2 = ((seq_t)1) << n;
    /* use seq_t arithmetic throughout to avoid overflow in partial
       results of intermediate 64‑bit builds */
    return ((seq_t)20) * pow2 - ((seq_t)6) * ((seq_t)n) - ((seq_t)16);
}
