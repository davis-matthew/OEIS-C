/*
 * A044870  Positive integers having the same number of base-14 runs of odd
 *          length as even.
 * https://oeis.org/A044870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n‑th positive integer whose base‑14 expansion *
 *            consists of an equal number of maximal runs of odd length and
 *            maximal * runs of even length
 * Range:     u8 n <= 16, u16 n <= 386, u32 n <= 5440, u64 n <= 5440,
 *            u128 n <= 5440, big any n
 * Validated: standard, pass, 61 terms through n = 5093 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Count the runs in the base‑14 expansion of x.
 * Return true if the number of odd‑length runs equals the number of
 * even‑length runs.  x is assumed to be > 0. */
static bool runs_match(idx_t x)
{
    int odd  = 0;
    int even = 0;
    int run_len = 1;
    int prev_digit = x % 14;
    x /= 14;

    while (x > 0) {
        int digit = x % 14;
        if (digit == prev_digit) {
            ++run_len;
        } else {
            if (run_len & 1) ++odd; else ++even;
            run_len = 1;
            prev_digit = digit;
        }
        x /= 14;
    }
    /* finalize the last run */
    if (run_len & 1) ++odd; else ++even;

    return odd == even;
}

/* Public function: return the n-th term of the sequence.
 * n must be >= 1; a(0) is undefined and returns 0. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t candidate = 1;
    idx_t found = 0;

    while (1) {
        if (runs_match(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
