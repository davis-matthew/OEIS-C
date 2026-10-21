/*
 * A213636  Remainder when n is divided by its least nondivisor.
 * https://oeis.org/A213636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1807, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   least_nondivisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of A213636: a(n) = n mod m(n), where m(n) is the least integer > 1 that does not divide n. */
#include <stdbool.h>
#include "../utils/least_nondivisor.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1. */
    idx_t m = least_nondivisor(n);
    return (seq_t)n % (seq_t)m;
}
