/*
 * A044852  Positive integers having no fewer base-11 runs of even length than
 *          odd.
 * https://oeis.org/A044852
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Brute‑force generator for OEIS A044852.
   The nth term is the nth positive integer whose base‑11 representation
   has at least as many runs of even length as runs of odd length. */

#include <stdbool.h>

static inline bool has_property(idx_t m)
{
    /* Convert m to base 11 digits.  Digits are stored least
       significant first.  The number of digits is at most 19
       for 64‑bit m, so we use a small fixed array. */
    unsigned char digits[32];
    idx_t dcnt = 0;
    while (m > 0) {
        digits[dcnt++] = m % 11;
        m /= 11U;
    }
    /* Count runs when traversing digits from most to least
       significant. */
    idx_t even_runs = 0, odd_runs = 0;
    unsigned char cur = digits[dcnt - 1];
    idx_t run_len = 1;
    for (int i = (int)dcnt - 2; i >= 0; --i) {
        if (digits[i] == cur) {
            ++run_len;
        } else {
            if ((run_len & 1) == 0)
                ++even_runs;
            else
                ++odd_runs;
            cur = digits[i];
            run_len = 1;
        }
    }
    /* Final run. */
    if ((run_len & 1) == 0)
        ++even_runs;
    else
        ++odd_runs;

    return even_runs >= odd_runs;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 1; ; ++m) {
        if (has_property(m)) {
            ++count;
            if (count == n)
                return (seq_t)m;
        }
    }
}
