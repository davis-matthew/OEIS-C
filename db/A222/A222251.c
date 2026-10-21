/*
 * A222251  In the number n, replace all (decimal) digits '6' with '9' and
 *          vice versa.
 * https://oeis.org/A222251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace decimal digits 6<->9 in n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    unsigned char digits[20];
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 10);
        tmp /= 10;
    }
    seq_t result = 0;
    for (idx_t i = len; i > 0; --i) {
        unsigned char d = digits[i - 1];
        if (d == 6)
            d = 9;
        else if (d == 9)
            d = 6;
        result = result * 10 + d;
    }
    return result;
}
