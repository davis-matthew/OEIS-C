/*
 * A024638  n written in fractional base 6/5.
 * https://oeis.org/A024638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the decimal integer obtained by writing n in fractional
 *            base 6/5 (digits 0..5) and concatenating them
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 71, u64 n <= 503,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    const idx_t A = 6;
    const idx_t B = 5;
    unsigned char digits[256] = {0};
    int cnt = 0;
    idx_t temp = n;
    while (temp > 0) {
        unsigned char d = (unsigned char)(temp % A);
        digits[cnt++] = d;
        temp = ((temp - d) / A) * B;
    }
    seq_t result = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        result = result * 10 + digits[i];
    }
    return result;
}
