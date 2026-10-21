/*
 * A227146  Numbers that are congruent to {5, 11, 13, 14, 19} modulo 24.
 * https://oeis.org/A227146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*floor((n-1)/5) + {5,11,13,14,19}[(n-1)%5]
 * Range:     u8 n <= 54, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for invalid call with n == 0 (offset specification starts at n = 1). */
    if (n == 0) return 0;

    /* Offsets of accepted residues modulo 24 (sorted). */
    static const seq_t offs[5] = {5, 11, 13, 14, 19};

    idx_t block = (n - 1) / 5;           /* Complete blocks of size 5 */
    idx_t pos   = (n - 1) % 5;           /* Position inside the block (0..4) */

    /* Compute the nth term using the straightforward formula. */
    seq_t result = (seq_t)24 * (seq_t)block + offs[pos];
    return result;
}
