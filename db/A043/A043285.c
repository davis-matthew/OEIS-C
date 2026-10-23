/*
 * A043285  Maximal run length in base 11 representation of n.
 * https://oeis.org/A043285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9736, u16 n <= 9736, u32 n <= 9736, u64 n <= 9736,
 *            u128 n <= 9736, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: maximal run length of identical digits in base‑11 representation */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Handle the trivial case of n==0 (not part of the sequence but safe). */
    if (n == 0) return 0;

    /* Initialize with the least significant digit. */
    idx_t prev_digit = n % 11;
    n /= 11;

    seq_t max_len = 1;
    seq_t cur_len = 1;

    while (n > 0) {
        idx_t digit = n % 11;
        if (digit == prev_digit) {
            ++cur_len;
            if (cur_len > max_len)
                max_len = cur_len;
        } else {
            cur_len = 1;
            prev_digit = digit;
        }
        n /= 11;
    }

    return max_len;
}
