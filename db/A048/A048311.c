/*
 * A048311  Numbers whose base-14 expansions, read from left to right, have
 *          run lengths that strictly increase.
 * https://oeis.org/A048311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration: numbers with strictly increasing run-lengths in base-14 */
#include <stdbool.h>

static bool is_valid_base14(seq_t n)
{
    if (n == 0) return true;
    int digits[128];
    int len = 0;
    seq_t tmp = n;
    while (tmp != 0) {
        digits[len++] = (int)(tmp % 14);
        tmp /= 14;
    }
    int prev_len = 0;
    int curr_len = 1;
    int curr_digit = digits[len - 1];
    for (int i = len - 2; i >= 0; --i) {
        int d = digits[i];
        if (d == curr_digit) {
            ++curr_len;
        } else {
            if (prev_len && prev_len >= curr_len) return false;
            prev_len = curr_len;
            curr_digit = d;
            curr_len = 1;
        }
    }
    if (prev_len && prev_len >= curr_len) return false;
    return true;
}

seq_t a(idx_t n)
{
    seq_t num = 0;
    idx_t count = 0;
    while (1) {
        if (is_valid_base14(num)) {
            ++count;
            if (count == n + 1) {
                return num;
            }
        }
        ++num;
    }
}
