/*
 * A213636  Remainder when n is divided by its least nondivisor.
 * https://oeis.org/A213636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1807, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9697 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A213636: a(n) = n mod m(n), where m(n) is the least integer > 1 that does not divide n. */
#include <stdbool.h>

static idx_t least_nondivisor(idx_t n)
{
    /* Find the smallest integer k > 1 such that n % k != 0. */
    for (idx_t k = 2; ; ++k) {
        if (n % k != 0) {
            return k;
        }
    }
}

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    idx_t m = least_nondivisor(n);
    return (seq_t)n % (seq_t)m;
}
