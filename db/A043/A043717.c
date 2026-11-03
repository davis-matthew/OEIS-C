/*
 * A043717  Numbers whose base-14 representation has an odd number of runs.
 * https://oeis.org/A043717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the nth positive integer whose base‑14 representation has
 *            an odd number of runs. Computed by linear search
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9880 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Returns true iff x in base 14 has an odd number of runs. */
static inline bool has_odd_runs(seq_t x)
{
    unsigned changes = 0;          /* parity of the number of digit changes */
    unsigned prev_digit = 0;
    bool have_prev = false;
    seq_t y = x;

    while (y > (seq_t)0) {
        unsigned digit = (unsigned)(y % (seq_t)14); /* extract least‑significant digit */
        if (have_prev && digit != prev_digit)
            changes ^= 1;           /* change of digit toggles parity */
        prev_digit = digit;
        have_prev = true;
        y /= (seq_t)14;             /* remove the last digit */
    }
    return changes == 0;            /* odd runs <=> even number of changes */
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* undefined for n<1, but safe return */

    idx_t found = 0;
    seq_t candidate = 1;

    while (found < n) {
        if (has_odd_runs(candidate)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
    return candidate; /* unreachable */
}
