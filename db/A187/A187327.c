/*
 * A187327  a(n) = floor(n/5) + floor(2n/5) + floor(3n/5).
 * https://oeis.org/A187327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/5)+floor(2n/5)+floor(3n/5)
 * Range:     u8 n <= 120, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute q = n/5 and r = n%5 to avoid overflow when multiplying
       large numbers by 2 or 3.  The result is 6*q + floor(2r/5) + floor(3r/5). */
    idx_t q = n / 5;
    idx_t r = n % 5;
    seq_t res = (seq_t)q * 6;
    res += (seq_t)((r * 2) / 5);
    res += (seq_t)((r * 3) / 5);
    return res;
}
