/*
 * A043634  Numbers whose base-9 representation has exactly 5 runs.
 * https://oeis.org/A043634
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search base‑9 numbers for exactly 5 runs. */
#include <stdbool.h>

static bool has_five_runs(seq_t x)
{
    int run_count = 0;
    int prev_digit = -1;
    while (x > 0) {
        int digit = (int)(x % 9);
        if (digit != prev_digit) {
            ++run_count;
            prev_digit = digit;
            if (run_count > 5) return false;
        }
        x /= 9;
    }
    return run_count == 5;
}

seq_t a(idx_t n)
{
    /* Count until the n‑th valid number is found. */
    idx_t found = 0;
    seq_t x = 0;
    while (1) {
        if (has_five_runs(x)) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
}
