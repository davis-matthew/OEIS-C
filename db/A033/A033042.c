/*
 * A033042  Sums of distinct powers of 5.
 * https://oeis.org/A033042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ∑ bit_i(n)·5^i (bit_i(n) is the i‑th binary digit of n)
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 0;
    seq_t pow = 1;
    idx_t m = n;
    while (m)
    {
        if (m & 1) res += pow;
        m >>= 1;
        if (m) pow *= 5;
    }
    return res;
}
