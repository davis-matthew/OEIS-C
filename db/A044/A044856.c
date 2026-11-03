/*
 * A044856  Positive integers having no fewer base-15 runs of even length than
 *          odd.
 * https://oeis.org/A044856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A044856:
   Positive integers having no fewer base‑15 runs of even length than odd.
   The function enumerates natural numbers, counts runs of even and odd
   length in base 15, and returns the n‑th number that satisfies the
   condition. */

#include <stdbool.h>
#include <stddef.h>

static bool has_no_fewer_even_runs(seq_t m)
{
    /* Convert m to base‑15 digits (least significant first). */
    const seq_t fifteen = (seq_t)15;
    seq_t digits[200];          /* ample capacity for any reasonably sized m */
    idx_t len = 0;

    while (m > (seq_t)0) {
        digits[len++] = m % fifteen;
        m /= fifteen;
    }

    /* Count runs of even and odd lengths. */
    idx_t runs_even = 0, runs_odd = 0;
    if (len == 0) return false;   /* should not happen for positive m */

    seq_t prev = digits[len - 1];
    idx_t run_len = 1;

    for (idx_t i = len - 1; i > 0; --i) {
        seq_t d = digits[i - 1];
        if (d == prev) {
            ++run_len;
        } else {
            if (run_len & (idx_t)1) ++runs_odd;
            else ++runs_even;
            prev = d;
            run_len = 1;
        }
    }

    /* Final run. */
    if (run_len & (idx_t)1) ++runs_odd;
    else ++runs_even;

    return runs_even >= runs_odd;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = (seq_t)1;

    for (;;) {
        if (has_no_fewer_even_runs(m)) {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
