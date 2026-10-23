/*
 * A111707  a(n) = Sum_{k = 1..ceiling(w/2)} d(k) * d(w+1-k), where (d(1),
 *          ..., d(w)) is the decimal expansion of n.
 * https://oeis.org/A111707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = sum_{k=1..ceil(w/2)} d(k) * d(w+1-k)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    int digits[20];
    idx_t x = n;
    int len = 0;
    while (x > 0) {
        digits[len++] = (int)(x % 10);
        x /= 10;
    }
    int half = (len + 1) / 2;
    for (int i = 0; i < half; ++i) {
        result += (seq_t)digits[i] * (seq_t)digits[len - i - 1];
    }
    return result;
}
