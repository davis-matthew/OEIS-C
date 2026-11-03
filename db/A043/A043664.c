/*
 * A043664  Numbers whose base-14 representation has exactly 3 runs.
 * https://oeis.org/A043664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th integer whose base‑14 representation has
 *            exactly 3 runs. There is no simple closed form; we compute it by
 *            brute‑force search
 * Range:     u8 n <= 43, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static int digit_runs_base14(seq_t x) {
    int runs = 0;
    seq_t prev = (seq_t)-1;          /* sentinel that can’t be a digit (0‑13) */
    while (x > 0) {
        seq_t d = x % (seq_t)14;      /* current least significant digit */
        if (runs == 0 || d != prev) {
            runs++;
            prev = d;
            if (runs > 3) return runs;   /* early exit if already more than 3 runs */
        }
        x /= (seq_t)14;
    }
    return runs;
}

seq_t a(idx_t n) {
    seq_t val = 1;                /* start from the smallest positive integer */
    idx_t count = 0;

    for (;;) {
        if (digit_runs_base14(val) == 3) {
            ++count;
            if (count == n) return val;
        }
        ++val;
    }
}
