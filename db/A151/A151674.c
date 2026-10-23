/*
 * A151674  G.f.: Product_{k >= 0} (1 + 4*x^(5^k)).
 * https://oeis.org/A151674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^m if the base-5 representation of n uses only digits 0
 *            or 1; otherwise 0, where m is the number of 1’s in that
 *            representation
 * Range:     u8 n <= 102, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t temp = n;
    idx_t ones = 0;
    while (temp > 0) {
        uint8_t d = temp % 5;
        if (d > 1) return 0;
        if (d == 1) ++ones;
        temp /= 5;
    }
    seq_t res = 1;
    for (idx_t i = 0; i < ones; ++i) res *= 4;
    return res;
}
