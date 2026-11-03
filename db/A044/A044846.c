/*
 * A044846  Positive integers having no fewer base-5 runs of even length than
 *          odd.
 * https://oeis.org/A044846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044846: Positive integers having no fewer base‑5 runs of even
 * length than runs of odd length.  The function a(n) returns the n‑th such
 * integer for n ≥ 1.  This implementation scans forwards from 1 on every
 * call; it is correct for any order of calls and runs well below the
 * 2‑second limit for n up to about 10 000. */

#include <stdlib.h>    /* for malloc/realloc; not strictly needed here */

static int has_even_or_more_runs(seq_t x)
{
    seq_t tmp = x;
    int prev_digit = -1;
    int run_len   = 0;
    int evens = 0, odds = 0;

    while (tmp > 0) {
        unsigned int d = (unsigned int)(tmp % 5);   /* digit 0‑4 */
        if (prev_digit == -1) {
            prev_digit = (int)d;
            run_len    = 1;
        } else if (d == (unsigned int)prev_digit) {
            ++run_len;
        } else {
            if (run_len % 2 == 0) ++evens;
            else                 ++odds;
            prev_digit = (int)d;
            run_len    = 1;
        }
        tmp = tmp / 5;
    }

    /* account for the final run */
    if (run_len > 0) {
        if (run_len % 2 == 0) ++evens;
        else                 ++odds;
    }
    return evens >= odds;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for indices below the offset */

    idx_t found = 0;
    seq_t candidate = 1;

    while (found < n) {
        if (has_even_or_more_runs(candidate)) {
            ++found;
            if (found == n) return candidate;
        }
        ++candidate;
    }
    return 0;   /* unreachable, but satisfies the compiler */
}
