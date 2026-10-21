/*
 * A097251  Numbers whose set of base 5 digits is {0,4}.
 * https://oeis.org/A097251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 4 * \sum_{i\ge0} bit_i(n) * 5^i (OEIS A097251)
 * Range:     u8 n <= 7, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    unsigned __int128 pow5 = 1;   /* 5^i held in 128‑bit temp to avoid overflow */
    while (n) {
        if (n & 1) {
            result += (seq_t)(4 * pow5);
        }
        n >>= 1;
        pow5 *= 5;
    }
    return result;
}
