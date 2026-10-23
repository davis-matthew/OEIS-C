/*
 * A043090  Every string of 2 consecutive base 4 digits contains exactly 2
 *          distinct numbers.
 * https://oeis.org/A043090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A043090: n‑th nonnegative integer whose base‑4 representation has no consecutive equal digits */
#include <stdbool.h>

static bool no_consecutive_equal_digits(seq_t x)
{
    if (x < (seq_t)4) return true;        /* 0–3 are always valid */
    seq_t prev = x % (seq_t)4;
    x /= (seq_t)4;
    while (x > (seq_t)0) {
        seq_t d = x % (seq_t)4;
        if (d == prev) return false;     /* found equal neighbors */
        prev = d;
        x /= (seq_t)4;
    }
    return true;
}

seq_t a(idx_t n)
{
    seq_t val = 0;
    idx_t cnt = 0;
    for (;; ++val) {
        if (no_consecutive_equal_digits(val)) {
            if (cnt == n) return val;
            ++cnt;
        }
    }
}
