/*
 * A033045  Sums of distinct powers of 8.
 * https://oeis.org/A033045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{i≥0} bit_i(n) * 8^i, where bit_i(n) is the i‑th binary
 *            digit of n
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow = 1;
    idx_t x = n;
    while (x)
    {
        if (x & 1) res += pow;   /* add 8^i if the i‑th bit is 1 */
        pow *= 8;                /* advance to the next power of 8 */
        x >>= 1;
    }
    return res;
}
