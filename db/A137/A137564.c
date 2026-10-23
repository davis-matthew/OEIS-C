/*
 * A137564  a(n) is the number formed by removing from n all duplicate digits
 *          except the leftmost copy of each.
 * https://oeis.org/A137564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number formed by removing from n all duplicate digits except the
 * leftmost copy of each digit (decimal representation). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    unsigned seen = 0;
    seq_t result = 0;
    unsigned char digits[20];
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 10);
        temp /= 10;
    }
    for (int i = len - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        if (!(seen & (1u << d))) {
            seen |= (1u << d);
            result = result * 10 + d;
        }
    }
    return result;
}
