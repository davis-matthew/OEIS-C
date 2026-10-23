/*
 * A167190  a(n) = 1 + 85*n + 2232*n^2 + 15276*n^3.
 * https://oeis.org/A167190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 85*n + 2232*n^2 + 15276*n^3
 * Range:     u8 none, u16 n <= 1, u32 n <= 65, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to seq_t once to avoid repeated conversions */
    seq_t n_s = (seq_t)n;

    /* Compute each term of the polynomial carefully to avoid overflow
       in intermediate steps (seq_t is at least 64 bits). */
    seq_t term1 = 85 * n_s;                     /* 85*n */
    seq_t term2 = 2232 * n_s * n_s;             /* 2232*n^2 */
    seq_t term3 = 15276 * n_s * n_s * n_s;      /* 15276*n^3 */

    return 1 + term1 + term2 + term3;
}
