/*
 * A170987  Carryless product n X n in base 6.
 * https://oeis.org/A170987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = carryless product of n with itself in base 6
 * Range:     u8 n <= 35, u16 n <= 257, u32 n <= 7775, u64 n <= 7775,
 *            u128 n <= 7775, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* extract base‑6 digits of n (least significant first) */
    uint8_t dig[32] = {0};
    idx_t len = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        len = 1;
    } else {
        while (tmp > 0) {
            dig[len++] = (uint8_t)(tmp % 6);
            tmp /= 6;
        }
    }

    /* convolution of digits with themselves; result length = 2*len-1 */
    int res[64] = {0};
    idx_t max_len = 2 * len - 1;

    for (idx_t i = 0; i < len; ++i) {
        for (idx_t j = 0; j < len; ++j) {
            res[i + j] += dig[i] * dig[j];
        }
    }

    /* reduce each coefficient modulo 6 */
    for (idx_t k = 0; k < max_len; ++k) {
        res[k] %= 6;
    }

    /* turn digit vector into integer (most significant first) */
    seq_t val = 0;
    for (idx_t k = max_len; k-- > 0; ) {
        val = val * (seq_t)6 + (seq_t)res[k];
    }

    return val;
}
