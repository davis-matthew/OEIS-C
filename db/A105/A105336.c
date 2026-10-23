/*
 * A105336  a(n) = n*(n+1)/2 mod 128.
 * https://oeis.org/A105336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1)/2 mod 128
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence has period 256, so reduce n modulo 256 to avoid overflow. */
    seq_t m = (seq_t)(n & 255ULL);        /* m = n mod 256, 0 ≤ m < 256 */
    seq_t res = (m * (m + 1) >> 1) & 127; /* (m*(m+1)/2) mod 128 */
    return res;
}
