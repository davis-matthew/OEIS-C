/*
 * A037975  Maximal base 3 run length is 4.
 * https://oeis.org/A037975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers whose base‑3 representation has a maximal run of 4 identical digits.
   The function enumerates integers and returns the nth such number. */
#include <stdbool.h>

static bool has_run4(idx_t x) {
    seq_t max_run = 0;
    seq_t cur_run = 0;
    int prev_digit = -1;
    while (x > 0) {
        int digit = (int)(x % 3);
        if (digit == prev_digit) {
            cur_run++;
        } else {
            cur_run = 1;
            prev_digit = digit;
        }
        if (cur_run > max_run) max_run = cur_run;
        x /= 3;
    }
    return (max_run == 4);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;  /* offset starts at 1 */
    seq_t count = 0;
    idx_t num = 0;
    while (1) {
        ++num;
        if (has_run4(num)) {
            ++count;
            if (count == n) return (seq_t)num;
        }
    }
}
