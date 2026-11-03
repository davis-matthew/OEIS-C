/*
 * A044900  Base 14 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044900
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose base‑14 representations have run
   lengths alternating even, odd, even, ... starting with even.
   a(n) is the nth such integer. */
#include <stdbool.h>
#include <stdint.h>

/* Test whether a base‑14 representation of num has
   alternating run lengths even, odd, even, ... starting with even.
   The function works for all seq_t sizes supported by the build. */
static bool alt_base14_runs_even_odd(seq_t num) {
    if (num == 0) return false;          /* zero is not part of the sequence */

    /* Extract base‑14 digits into an array (least significant first). */
    int digits[128];
    int d = 0;
    seq_t t = num;
    while (t > 0) {
        digits[d++] = (int)(t % 14);
        t /= 14;
    }

    /* Process runs from most significant to least. */
    int last_digit = digits[d - 1];
    int len = 1;
    int expect_even = 1;                 /* first run must be even */

    for (int i = d - 2; i >= 0; --i) {
        int digit = digits[i];
        if (digit == last_digit) {
            len++;
        } else {
            int parity = len & 1;        /* 0 = even, 1 = odd */
            if ((expect_even && parity != 0) ||
                (!expect_even && parity != 1))
                return false;
            expect_even ^= 1;            /* alternate expectation */
            last_digit = digit;
            len = 1;
        }
    }

    /* Check the final run. */
    int parity = len & 1;
    if ((expect_even && parity != 0) ||
        (!expect_even && parity != 1))
        return false;

    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    seq_t candidate = 1;

    while (1) {
        if (alt_base14_runs_even_odd(candidate)) {
            count++;
            if (count == n) return candidate;
        }
        candidate++;
    }
}
