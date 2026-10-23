/*
 * A132269  a(n) = Product_{k>=0} (1 + floor(n/2^k)).
 * https://oeis.org/A132269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k>=0} (1 + floor(n/2^k))
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 159, u64 n <= 1727,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    while (n > 0) {
        result *= ((seq_t)n + 1);
        n >>= 1;
    }
    return result;
}
