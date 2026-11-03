/*
 * A044901  Base 15 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th positive integer whose base‑15 expansion has run
 *            lengths that alternate even, odd, even, … (the first run must be
 *            even)
 * Range:     u8 n <= 28, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

/* Test whether x > 0 and its base‑15 digits form runs that
   alternate even, odd, even, … starting with an even run. */
static bool base15_alternating_runs(seq_t x)
{
    if (x == 0)             /* not considered */
        return false;

    /* Extract digits of x in base 15, least significant first */
    unsigned char digits[256];
    size_t len = 0;
    seq_t t = x;
    while (t > 0) {
        digits[len++] = (unsigned char)(t % 15);
        t /= 15;
    }

    /* A single digit cannot satisfy the requirement
       (first run would have length 1, odd). */
    if (len == 1)
        return false;

    /* Process runs from most significant digit downward.
       cur holds the current run digit, run_len the length of
       the current run, and expect_even indicates whether
       the current run should have even length. */
    unsigned char cur = digits[len - 1];
    size_t run_len = 1;
    bool expect_even = true;

    /* Iterate from the second most significant digit down to the least. */
    for (int i = (int)len - 2; ; --i) {
        unsigned char d = digits[i];
        if (d == cur) {
            ++run_len;
        } else {
            /* Check parity of finished run */
            if (!( (expect_even && (run_len % 2 == 0)) ||
                   (!expect_even && (run_len % 2 == 1)) ))
                return false;

            /* Prepare for next run */
            expect_even = !expect_even;
            cur = d;
            run_len = 1;
        }
        if (i == 0)
            break;
    }

    /* Check the final run */
    return (expect_even && (run_len % 2 == 0)) ||
           (!expect_even && (run_len % 2 == 1));
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;              /* undefined, but safe fallback */

    seq_t cur = 1;
    idx_t count = 0;

    while (1) {
        if (base15_alternating_runs(cur)) {
            ++count;
            if (count == n)
                return cur;
        }
        ++cur;
    }
}
