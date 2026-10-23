/*
 * A043641  Numbers whose base-10 representation has exactly 5 runs.
 * https://oeis.org/A043641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 30, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: iterate through integers, counting those whose decimal
 * representation has exactly 5 runs (i.e., 4 transitions between
 * successive digits). The nth such integer is returned. */
#include <stdbool.h>

static inline bool has_five_runs(seq_t n) {
    /* A number cannot have 5 runs if it has fewer than 5 digits. */
    if (n < 10000) return false;       // less than 5 digits

    int runs = 1;
    seq_t prev = n % 10;
    n /= 10;
    while (n > 0) {
        seq_t d = n % 10;
        if (d != prev) ++runs;
        prev = d;
        n /= 10;
    }
    return runs == 5;
}

seq_t a(idx_t n) {
    idx_t found = 0;
    seq_t x = 1;
    while (1) {
        if (has_five_runs(x)) {
            ++found;
            if (found == n) return x;
        }
        ++x;
    }
}
