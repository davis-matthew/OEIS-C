/*
 * A044895  Base 9 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 49 terms through n = 49 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A044895: Base‑9 run lengths alternate even, odd, even, …
 * A number n is in the sequence if in its base‑9 representation the
 * lengths of consecutive runs of equal digits alternate between even
 * and odd, beginning with a run of even length.
 * The sequence is indexed starting at 1 (a(1)=10).
 */

#include <stdbool.h>
#include <stddef.h>

static inline bool has_alternating_runs(seq_t x)
{
    /* Extract digits of x in base 9 (least‑significant first). */
    seq_t digits[64];
    size_t len = 0;
    seq_t tmp = x;
    while (tmp > 0) {
        digits[len++] = tmp % 9;
        tmp /= 9;
    }
    if (len == 0)   /* only occurs for x == 0, which never appears */
        return false;

    /* Scan runs from most‑significant to least‑significant. */
    seq_t prev = digits[len - 1];
    size_t run_len = 1;
    int expected_parity = 0;          /* 0 = even, 1 = odd    */

    for (size_t i = len - 1; i > 0; --i) {
        seq_t cur = digits[i - 1];
        if (cur == prev) {
            run_len++;
        } else {
            /* Run has finished; check its parity. */
            if ((run_len & 1) != expected_parity)
                return false;
            expected_parity ^= 1;  /* toggle expected parity */
            prev = cur;
            run_len = 1;
        }
    }

    /* Final run */
    if ((run_len & 1) != expected_parity)
        return false;

    return true;
}

seq_t a(idx_t n)
{
    /* The sequence starts with a(1) = 10.  Scan consecutive integers
       until the n‑th valid one is found. */
    seq_t value = 10;          /* first term */
    idx_t count = 1;           /* count of terms found so far */

    if (n == 1)
        return value;

    while (1) {
        value++;
        if (has_alternating_runs(value)) {
            count++;
            if (count == n)
                return value;
        }
    }
}
