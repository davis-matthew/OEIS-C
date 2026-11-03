/*
 * A107085  Numbers whose largest digit in its decimal representation is equal
 *          to its digital root.
 * https://oeis.org/A107085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 7284, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9932 (2026-11-01)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: return the nth integer (1‑indexed) whose largest digit equals its digital root. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Helper to obtain the largest decimal digit of a seq_t value. */
static inline seq_t largest_digit(seq_t v)
{
    seq_t dmax = 0;
    while (v != 0) {
        seq_t d = v % 10;
        if (d > dmax) dmax = d;
        v /= 10;
    }
    return dmax;
}

/* Compute the digital root of x.  For x == 0 this returns 0; otherwise it
   repeatedly sums the digits until a single decimal digit remains.  */
static seq_t digital_root(seq_t x)
{
    if (x == (seq_t)0)
        return (seq_t)0;
    seq_t s = digit_sum(x);
    while (s >= (seq_t)10)
        s = digit_sum(s);
    return s;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t cand   = 0;

    while (found < n) {
        seq_t v = (seq_t)cand;
        if (largest_digit(v) == digital_root(v))
            ++found;
        ++cand;
    }

    return (seq_t)(cand - 1);
}
