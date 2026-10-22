/*
 * A006072  Numbers with mirror symmetry about middle.
 * https://oeis.org/A006072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = digit-wise application of A000578 (cube) to A118594(n),
 *            i.e. the n-th base-3 palindrome with digit 2 replaced by 8, read
 *            in decimal
 * Range:     u8 n <= 8, u16 n <= 26, u32 n <= 404, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t pos = n - 1;

    /* Find the length of the base-3 palindrome */
    idx_t count = 0;
    idx_t len = 1;
    while (true) {
        idx_t pals_in_len;
        if (len == 1) {
            pals_in_len = 3;
        } else {
            idx_t half_len = (len + 1) / 2;
            pals_in_len = 2;
            for (idx_t i = 1; i < half_len; i++) {
                pals_in_len *= 3;
            }
        }
        if (pos < count + pals_in_len) {
            break;
        }
        count += pals_in_len;
        len++;
    }

    if (len == 1) {
        seq_t digit = (seq_t)pos;
        return digit * digit * digit;
    }

    idx_t pos_in_len = pos - count;
    idx_t half_len = (len + 1) / 2;

    idx_t block = 1;
    for (idx_t i = 1; i < half_len; i++) {
        block *= 3;
    }

    unsigned half_digits[64];
    idx_t remaining = pos_in_len;

    half_digits[0] = (unsigned)(1 + remaining / block);
    remaining %= block;

    for (idx_t i = 1; i < half_len; i++) {
        block /= 3;
        half_digits[i] = (unsigned)(remaining / block);
        remaining %= block;
    }

    /* Build result: cube each digit (0->0, 1->1, 2->8) and concatenate */
    seq_t result = 0;
    for (idx_t i = 0; i < half_len; i++) {
        unsigned d = half_digits[i];
        result = result * 10 + (seq_t)(d * d * d);
    }

    int start_idx = (len % 2 == 0) ? (int)half_len - 1 : (int)half_len - 2;
    for (int i = start_idx; i >= 0; i--) {
        unsigned d = half_digits[i];
        result = result * 10 + (seq_t)(d * d * d);
    }

    return result;
}
