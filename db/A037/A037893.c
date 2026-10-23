/*
 * A037893  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*7^i} is the base 7
 *          representation of n and e(i) are the digits d(i) in reverse order.
 * https://oeis.org/A037893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = (1/2) * ∑_{i=0}^{k} |d_i - d_{k-i}| * where d_i are the
 *            base‑7 digits of n (least significant first) * and k is the
 *            index of the most significant digit
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* convert n to base-7 digits, least significant first */
    uint8_t digits[25];  /* maximum number of base-7 digits for 64‑bit n is 23 */
    size_t len = 0;
    idx_t temp = n;
    if (temp == 0) {
        digits[0] = 0;
        len = 1;
    } else {
        while (temp > 0) {
            digits[len++] = temp % 7;
            temp /= 7;
        }
    }

    /* compute the sum of absolute differences between mirrored digits */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        uint8_t d1 = digits[i];
        uint8_t d2 = digits[len - 1 - i];
        /* absolute value without calling abs to stay within allowed ops */
        seq_t diff = (d1 > d2) ? (seq_t)(d1 - d2) : (seq_t)(d2 - d1);
        sum += diff;
    }

    /* final division by 2 (the sum is guaranteed even) */
    return sum / 2;
}
