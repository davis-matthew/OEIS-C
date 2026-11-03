/*
 * A044893  Base 7 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044893:
   a(n) is the n‑th positive integer whose base‑7 representation has
   run‑lengths alternating in parity, starting with even.
   (e.g. 8 → 11₇, runs 2 even; 58 → 112₇, runs 2 even, 1 odd, etc.) */

#include <stdbool.h>
#include <stddef.h>

/* Test whether x satisfies the alternating run‑length property. */
static bool a044893_valid(seq_t x)
{
    /* 8 is the smallest term; any smaller integer cannot work */
    if (x < 8)
        return false;

    /* Extract base‑7 digits (least significant first). 200 digits is
       plenty for the values we will reach for n ≤ 10000. */
    uint8_t digits[200];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (uint8_t)(t % 7);
        t /= 7;
        /* safety guard – should never hit, but avoid overflow */
        if (len >= sizeof(digits) / sizeof(digits[0]))
            return false;
    }

    /* Scan the digits from most significant to least. */
    bool want_even = true;          /* first run must be even */
    size_t run_len = 1;
    uint8_t prev = digits[len - 1];

    for (int i = (int)len - 2; i >= 0; --i) {
        uint8_t cur = digits[i];
        if (cur == prev) {
            ++run_len;
        } else {
            /* Check parity of the run that just ended. */
            if ((run_len & 1) != (want_even ? 0 : 1))
                return false;
            want_even = !want_even;  /* toggle expected parity */
            prev = cur;
            run_len = 1;
        }
    }

    /* Check parity of the last run. */
    return ((run_len & 1) == (want_even ? 0 : 1));
}

/* Return the nth term of the sequence (n ≥ 1).  If n < 1, 0 is returned. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    idx_t count = 0;
    for (idx_t x = 8; ; ++x) {          /* start at the first possible term */
        if (a044893_valid(x)) {
            ++count;
            if (count == n)
                return (seq_t)x;
        }
    }
}
