/*
 * A169916  Squares in carryless arithmetic mod 10 with addition and
 *          multiplication of digits both defined to be addition mod 10.
 * https://oeis.org/A169916
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n * n in carryless base‑10 arithmetic where digit
 *            multiplication and addition are performed modulo 10 with no
 *            carries
 * Range:     u8 n <= 11, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* a(0) = 0 */

    uint8_t digits[20];            /* idx_t is 64‑bit → at most 20 decimal digits */
    size_t L = 0;
    idx_t temp = n;
    while (temp != 0) {
        digits[L++] = temp % 10;
        temp /= 10;
    }

    uint8_t out[40] = {0};         /* quadratic size: at most 2*L digits */
    size_t outCount = 2 * L;

    for (size_t i = 0; i < L; ++i) {
        for (size_t j = 0; j < L; ++j) {
            uint8_t prod = digits[i] + digits[j];
            if (prod >= 10) prod -= 10;      /* (digits[i] + digits[j]) mod 10 */
            out[i + j] += prod;              /* addition (pending mod 10) */
        }
    }

    /* reduce each position modulo 10 (no carries). */
    for (size_t k = 0; k < outCount; ++k)
        out[k] %= 10;

    /* trim leading zeros */
    while (outCount > 0 && out[outCount - 1] == 0)
        --outCount;

    if (outCount == 0) return 0;

    seq_t result = 0;
    for (size_t k = outCount; k > 0; --k)
        result = result * 10 + (seq_t)out[k - 1];

    return result;
}
