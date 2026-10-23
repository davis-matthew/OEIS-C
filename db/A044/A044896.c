/*
 * A044896  Base 10 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers that, in base‑10, have run lengths alternating
 * even, odd, even, odd, … starting with an even run. */

#include <stddef.h>
#include <stdbool.h>

/* Test whether a number satisfies the run‑length parity rule. */
static bool good(seq_t x) {
    if (x == 0) return false;
    /* Extract digits (least‑significant first). */
    unsigned char digits[64];
    size_t len = 0;
    seq_t t = x;
    while (t != 0) {
        digits[len++] = (unsigned char)(t % (seq_t)10);
        t /= (seq_t)10;
    }
    /* Process runs from most‑significant to least‑significant. */
    size_t i = len;
    size_t run_index = 0; /* 0 for first run */
    while (i > 0) {
        unsigned char curr = digits[--i];
        size_t run_len = 1;
        while (i > 0 && digits[i - 1] == curr) {
            --i;
            ++run_len;
        }
        /* First run must be even, second odd, etc. */
        if ((run_index % 2 == 0 && (run_len % 2 == 1)) ||
            (run_index % 2 == 1 && (run_len % 2 == 0))) {
            return false;
        }
        ++run_index;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t cand = 1;
    while (1) {
        if (good(cand)) {
            ++count;
            if (count == n) return cand;
        }
        ++cand;
    }
}
