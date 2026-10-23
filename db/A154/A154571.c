/*
 * A154571  Numbers that are congruent to {0, 3, 4, 5, 7, 8} mod 12.
 * https://oeis.org/A154571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 12*floor((n-1)/6) + offset[(n-1)%6] with offset = {0, 3,
 *            4, 5, 7, 8}
 * Range:     u8 n <= 128, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t offsets[6] = {0, 3, 4, 5, 7, 8};
    idx_t n_minus = n - 1;           /* n >= 1 */
    idx_t block   = n_minus / 6;     /* number of complete 12‑number blocks */
    idx_t pos     = n_minus % 6;     /* position within the current block */

    /* Compute the base value for the block and add the offset for the residue. */
    seq_t base = (seq_t)12 * (seq_t)block;
    return base + offsets[pos];
}
