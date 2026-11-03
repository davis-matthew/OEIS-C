/*
 * A043696  Numbers whose base-7 representation has an even number of runs.
 * https://oeis.org/A043696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A043696 - Numbers whose base‐7 representation has an even number of runs.
   Implementation: Linear search over natural numbers, counting runs in base‑7
   representation until the nth such number is found. */
#include <stdbool.h>
#include <stdint.h>

static bool has_even_runs_base7(seq_t x)
{
    if (x == 0) return false;             /* 0 has one run; exclude. */
    seq_t prev_digit = 0;
    bool first = true;
    unsigned long long runs = 0;
    while (x > 0) {
        seq_t d = x % 7;
        if (first || d != prev_digit) {
            ++runs;
            prev_digit = d;
            first = false;
        }
        x /= 7;
    }
    return (runs & 1ULL) == 0;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                   /* Undefined for n < 1. */
    seq_t count = 0;
    for (seq_t val = 1; ; ++val) {
        if (has_even_runs_base7(val)) {
            ++count;
            if (count == n) return val;
        }
    }
}
