/*
 * A170990  Carryless product n X n in base 9.
 * https://oeis.org/A170990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = carryless product of n with itself in base 9
 * Range:     u8 n <= 17, u16 n <= 728, u32 n <= 6560, u64 n <= 6560,
 *            u128 n <= 6560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The carryless product is computed by treating the base‑9 digits of n
 * as coefficients of a polynomial over ℤ/9ℤ, squaring it, and then
 * interpreting the resulting coefficients as a base‑9 number.
 */
#include <stddef.h>   /* size_t */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    /* Store base‑9 digits of n (least significant first). */
    idx_t tmp = n;
    /* 9^20 > 2^64, so 32 digits are sufficient for idx_t==uint64_t.
     * The problem constraints allow n in the range of 10000, so 32
     * digits are more than enough. */
    unsigned char digits[32];
    size_t nd = 0;
    while (tmp != 0) {
        digits[nd++] = tmp % 9;
        tmp /= 9;
    }

    /* Convolution of the digit vector with itself, modulo 9. */
    size_t result_len = 2 * nd;
    unsigned short res[64] = {0};

    for (size_t i = 0; i < nd; ++i) {
        unsigned char di = digits[i];
        for (size_t j = 0; j < nd; ++j)
            res[i + j] += di * digits[j];
    }

    for (size_t k = 0; k < result_len; ++k)
        res[k] %= 9;

    /* Convert the result digits (least significant first) back to a
     * decimal number. */
    seq_t value = 0;
    for (size_t k = result_len; k-- > 0;) {
        value = value * (seq_t)9 + (seq_t)res[k];
    }
    return value;
}
