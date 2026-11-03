/*
 * A044889  Base-3 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Validated: partial, pass, 48 terms through n = 48 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044889 – Base‑3 run lengths alternate: even, odd, even,... */

/* The implementation scans integers and checks their ternary representation.
   It returns the nth term (1‑based offset 1). */

#include <stddef.h>
#include <stdbool.h>

/* Return true iff the base‑3 representation of v has run lengths
   alternating even, odd, even,… starting with even.  */
static inline bool has_alternating_ternary_runs(idx_t v)
{
    /* Convert v to base‑3 digits (least significant first). */
    unsigned int digits[64];
    int len = 0;
    idx_t w = v;

    while (w > 0) {
        digits[len++] = (unsigned int)(w % 3);
        w /= 3;
    }
    if (len == 0)                /* v == 0 – not part of the sequence   */
        return false;

    /* Scan runs from most significant digit to least. */
    unsigned int cur = digits[len - 1];
    int run_len = 1;
    int expected_parity = 0;     /* 0 = even, 1 = odd */

    for (int i = len - 2; i >= 0; --i) {
        unsigned int d = digits[i];
        if (d == cur) {
            ++run_len;
        } else {
            if ((run_len & 1) != expected_parity)
                return false;
            expected_parity ^= 1;   /* flip parity for next run       */
            cur = d;
            run_len = 1;
        }
    }
    /* Final run */
    return ((run_len & 1) == expected_parity);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t val = 1; ; ++val) {
        if (has_alternating_ternary_runs(val)) {
            ++count;
            if (count == n)
                return (seq_t)val;
        }
    }
}
