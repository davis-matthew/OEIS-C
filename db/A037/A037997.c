/*
 * A037997  Numbers whose maximal base-9 run length is 2.
 * https://oeis.org/A037997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑9 representation has
 *            maximal run length exactly 2 (no digit repeats three or more
 *            times consecutively, but at least one run of two)
 * Range:     u8 n <= 42, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_max_run_2(seq_t x)
{
    int max_run = 0;
    int curr_run = 0;
    int prev_digit = -1;

    while (x > 0) {
        int d = (int)(x % 9);
        if (d == prev_digit) {
            curr_run++;
        } else {
            if (curr_run > max_run) max_run = curr_run;
            curr_run = 1;
            prev_digit = d;
        }
        x /= 9;
    }
    if (curr_run > max_run) max_run = curr_run;
    return max_run == 2;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (seq_t i = 1; ; ++i) {
        if (has_max_run_2(i)) {
            count++;
            if (count == n) return i;
        }
    }
}
