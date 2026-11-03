/*
 * A048326  Numbers whose base-15 expansions, read from left to right, have
 *          run lengths that strictly decrease.
 * https://oeis.org/A048326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Numbers whose base‑15 expansions, read from left to right, have
 *            run lengths that strictly decrease
 * Range:     u8 n <= 43, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Determine whether a given number has base‑15 run lengths that strictly decrease. */
static bool decreasing_runs(seq_t x) {
    if (x == (seq_t)0)
        return true;

    /* Buffer for up to 2048 base‑15 digits – more than enough for 10000th term. */
    unsigned char digits[2048];
    size_t len = 0;

    seq_t t = x;
    while (t > (seq_t)0) {
        digits[len++] = (unsigned char)(t % (seq_t)15);
        t /= (seq_t)15;
    }

    /* Single run is always allowed. */
    if (len == 1)
        return true;

    unsigned prev_run = (unsigned)-1;   /* Initialized larger than any possible run length. */
    unsigned cur_run = 1;
    unsigned cur_digit = digits[len - 1];

    for (int i = (int)len - 2; i >= 0; --i) {
        unsigned d = digits[i];
        if (d == cur_digit) {
            ++cur_run;
        } else {
            if (cur_run >= prev_run)   /* not strictly decreasing */
                return false;
            prev_run = cur_run;
            cur_digit = d;
            cur_run  = 1;
        }
    }

    /* Final run comparison. */
    if (cur_run >= prev_run)
        return false;
    return true;
}

/* Return the nth term of OEIS A048326 (offset 0). */
seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = (seq_t)0;

    while (true) {
        if (decreasing_runs(candidate)) {
            if (count == n)
                return candidate;
            ++count;
        }
        ++candidate;
    }
}
