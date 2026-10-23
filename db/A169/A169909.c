/*
 * A169909  a(n) = n+n in carryless arithmetic mod 9 in base 10.
 * https://oeis.org/A169909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = carryless addition of n with itself in base 10 modulo 9
 *            per digit
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t tmp = n;
    unsigned char digits[20];
    int len = 0;
    /* Extract decimal digits of n (least significant first) */
    while (tmp > 0) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = 0;
    /* Build the result from most significant digit to least */
    for (int i = len - 1; i >= 0; --i) {
        seq_t d = digits[i];
        seq_t mapped = (d * 2u) % 9u;   /* 2*d mod 9 */
        result = result * 10 + mapped;
    }
    return result;
}
