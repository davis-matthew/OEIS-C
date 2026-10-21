/*
 * A217558  Split-and-swap on 8-bit integers.
 * https://oeis.org/A217558
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = ((n & 0x0F) << 4) | ((n >> 4) & 0x0F)
 * Range:     u8 n <= 255, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Work only with the lowest 8 bits of n, as the split is on 8‑bit words. */
    seq_t x = (seq_t)n;
    x &= (seq_t)0xFF;                /* keep only bits 0–7 */

    seq_t low  = x & (seq_t)0x0F;    /* lower 4 bits */
    seq_t high = (x >> 4) & (seq_t)0x0F; /* upper 4 bits, shift right then mask */

    return (low << 4) | high;        /* swap halves */
}
