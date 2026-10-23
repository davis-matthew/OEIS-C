/*
 * A118312  Number of squares on infinite chessboard that a knight can reach
 *          in n moves from a fixed square.
 * https://oeis.org/A118312
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = -3 + 4*n + 7*n^2 + 4*sign((n-2)*(n-1))
 * Range:     u8 n <= 5, u16 n <= 96, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sign((n-2)*(n-1)) = 1 for n = 0 or n >= 3, 0 otherwise */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    /* Determine sign: 1 if n == 0 or n >= 3, else 0 */
    seq_t sign = (n == 0 || n >= 3) ? 1 : 0;
    /* Compute 7*n^2 + 4*n + 4*sign - 3  (unsigned arithmetic) */
    seq_t result = ((seq_t)7) * N * N + ((seq_t)4) * N + ((seq_t)4) * sign
                   - ((seq_t)3);
    return result;
}
