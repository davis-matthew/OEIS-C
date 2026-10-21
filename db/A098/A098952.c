/*
 * A098952  Integers divisible by their number of digits.
 * https://oeis.org/A098952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_count_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) is the nth non‑negative integer m
 * such that m is divisible by its number of decimal digits.
 * The function generates successive integers until it has
 * found the requested one.  The implementation does not
 * rely on any external utilities other than the basic
 * arithmetic operators that the definition obliges us
 * to use. */
#include <stdint.h>
#include "../utils/digit_count_seq_t.h"

seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    seq_t candidate = 0;   /* current integer to test */
    idx_t   seen = 0;      /* how many qualifying numbers found so far */

    while (1) {
        int d = digit_count_seq_t(candidate);
        /* d fits into an int; cast to seq_t for modulo operation */
        if (candidate % (seq_t)d == 0) {
            ++seen;
            if (seen == n)
                return candidate;
        }
        ++candidate;      /* move to next integer */
    }
}
