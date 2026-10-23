/*
 * A115419  Numbers having a 1 in position 3 of their binary expansion.
 * https://oeis.org/A115419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 8 + 16*floor((n-1)/8) + ((n-1)%8)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    idx_t k = n - 1;          /* zero‑based index */
    idx_t group = k / 8;      /* number of complete 8‑block groups */
    idx_t offset = k % 8;     /* position within current block */
    idx_t value = 8 + 16 * group + offset;
    return (seq_t)value;
}
