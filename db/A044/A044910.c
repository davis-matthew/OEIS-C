/*
 * A044910  Numbers whose base-9 run lengths alternate: odd, even, odd, ...
 * https://oeis.org/A044910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044910 – numbers whose base‑9 run lengths alternate odd, even, odd, ….
   Formula: a(n) = nth integer (n ≥ 1) whose base–9 digits form runs of alternating
   parity, starting with an odd run. This implementation searches sequentially. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Test if x satisfies the alternating run‑length property in base 9. */
static bool valid_base9_alt_runlen(seq_t x)
{
    /* Temporarily hold the base‑9 digits, least significant first. */
    int digits[200];
    int dcnt = 0;
    seq_t tmp = x;

    while (tmp > 0) {
        int d = (int)(tmp % 9);
        digits[dcnt++] = d;
        tmp /= 9;
    }
    if (dcnt == 0) return false;          /* x == 0 never appears in the sequence */

    bool expect_odd = true;               /* first run must be odd */
    int cur_digit = digits[dcnt - 1];
    int run_len   = 1;

    for (int i = dcnt - 2; i >= 0; --i) {
        int d = digits[i];
        if (d == cur_digit) {
            ++run_len;
        } else {
            /* finished a run – check its parity against expectation */
            if ((run_len & 1) != expect_odd) return false;
            expect_odd = !expect_odd;     /* toggle for the next run */
            cur_digit = d;
            run_len   = 1;
        }
    }
    /* final run: parity must match the current expectation */
    return ((run_len & 1) == expect_odd);
}

/* Return the n-th term of the sequence (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* safety for non‑positive indices */

    idx_t counted = 0;
    seq_t cand = 0;

    while (counted < n) {
        ++cand;
        if (valid_base9_alt_runlen(cand))
            ++counted;
    }
    return cand;
}
