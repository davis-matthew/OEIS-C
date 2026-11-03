/*
 * A043210  Numbers k such that 5 and 6 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 58 terms through n = 939 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043210: Numbers k such that 5 and 6 occur juxtaposed in
   the base-9 representation of k but not of k-1.
   Computed by brute‑force enumeration. */

#include <stdbool.h>

/* Test whether x has the substring "56" or "65" in base 9 (most significant first) */
static bool has56_seq(seq_t x)
{
    seq_t prev = (seq_t)(-1); /* sentinel for no previous digit */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)9);
        if ((d == 5 && prev == 6) || (d == 6 && prev == 5))
            return true;
        prev = d;
        x /= (seq_t)9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    seq_t k = 0;
    seq_t answer = 0;

    while (found < n) {
        ++k;                               /* next candidate */
        if (has56_seq(k) && !has56_seq(k - (seq_t)1)) {
            ++found;
            if (found == n) {
                answer = k;
                break;
            }
        }
    }
    return answer;
}
