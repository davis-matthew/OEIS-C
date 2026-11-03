/*
 * A043497  Numbers having one 2 in base 10.
 * https://oeis.org/A043497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 66 terms through n = 1922 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043497: numbers having exactly one digit 2 in base 10.
   The function iterates over natural numbers, counting those that
   contain a single '2', and returns the nth such number. */
#include <stdlib.h>
#include <stdbool.h>

/* Test whether a decimal representation of x contains exactly one digit 2 */
static inline bool has_one_digit_two(seq_t x)
{
    int cnt = 0;
    while (x != (seq_t)0) {
        seq_t d = x % (seq_t)10;
        if (d == (seq_t)2) {
            cnt++;
            if (cnt > 1)
                return false;        /* early exit */
        }
        x /= (seq_t)10;
    }
    return cnt == 1;
}

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    idx_t  found = 0;
    seq_t  candidate = (seq_t)1;

    while (found < n) {
        if (has_one_digit_two(candidate)) {
            found++;
            if (found == n)
                return candidate;
        }
        candidate++;
    }
    /* never reached for valid n */
    return (seq_t)0;
}
