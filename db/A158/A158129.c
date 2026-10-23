/*
 * A158129  a(n) = 100*n^2 - 2*n.
 * https://oeis.org/A158129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 100*n^2 - 2*n
 * Range:     u8 n <= 1, u16 n <= 25, u32 n <= 6553, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = n;              /* cast index to seq_t first */
    const seq_t HUNDRED = 100;
    const seq_t TWO      = 2;
    /* Compute 100*n^2 - 2*n avoiding overflow of intermediate 64-bit values
       (for 64‑bit builds n ≤ 10⁴ gives < 2¹⁶ cells; arbitrary precision builds
       handle larger n). */
    seq_t result = n_seq * ((HUNDRED * n_seq) - TWO);
    return result;
}
