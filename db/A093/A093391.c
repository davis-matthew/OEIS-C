/*
 * A093391  a(n) = floor(n/16) + floor((n+1)/16) + floor((n+2)/16) +
 *          floor((n+3)/16).
 * https://oeis.org/A093391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/16) + floor((n+1)/16) + floor((n+2)/16) +
 *            floor((n+3)/16)
 * Range:     u8 n <= 301, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t before each arithmetic operation to avoid mixing types */
    return ((seq_t)n) / 16
         + ((seq_t)n + 1) / 16
         + ((seq_t)n + 2) / 16
         + ((seq_t)n + 3) / 16;
}
