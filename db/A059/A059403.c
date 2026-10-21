/*
 * A059403  Quarter-squared applied twice.
 * https://oeis.org/A059403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(floor(n^2/4)^2 / 4)
 * Range:     u8 n <= 11, u16 n <= 45, u32 n <= 632, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for safe arithmetic */
    seq_t x = (seq_t)n;
    /* First quarter‑square: floor(n^2 / 4) */
    seq_t q = (x * x) / 4;
    /* Second quarter‑square: floor(q^2 / 4) */
    return (q * q) / 4;
}
