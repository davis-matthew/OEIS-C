/*
 * A059627  Generalized nim sum n + n + n + n in base 10; carryless
 *          multiplication 4 X n base 10.
 * https://oeis.org/A059627
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = carryless multiplication of 4 and n in base 10 (i.e.,
 *            each decimal digit d of n is replaced by (4*d) mod 10)
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Handle the special case n == 0 explicitly: result is 0.
    if (n == 0) return 0;

    /* Extract decimal digits of n into an array.
       n fits in idx_t, so it has at most 20 digits in base 10. */
    int digits[20];
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = temp % 10;
        temp /= 10;
    }

    seq_t res = 0;

    /* Process digits from most significant to least significant,
       building the result digit by digit. */
    for (int i = len - 1; i >= 0; --i) {
        /* Compute the carryless contribution of the current digit. */
        unsigned d = (digits[i] * 4) % 10;
        /* Append the digit: res = res * 10 + d. */
        res = res * 10 + d;
    }
    return res;
}
