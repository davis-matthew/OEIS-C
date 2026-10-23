/*
 * A124981  Odd numbers that are not the sum of 2 squares.
 * https://oeis.org/A124981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 5692, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124981: odd numbers that are NOT the sum of two squares
   (OEIS formula: a(n) = the nth odd integer with the property that it
   cannot be written as x^2 + y^2). */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t) */

/* Return true iff odd number x cannot be expressed as a sum of two squares.
   A number fails the two‑square representation iff it has a prime divisor
   p ≡ 3 (mod 4) raised to an odd power. */
static bool not_sum_two_squares_odd(seq_t x)
{
    /* small speed‑up: if x == 1, it is a sum of two squares (0^2+1^2) */
    if (x == 1)
        return false;

    /* compute floor(sqrt(x)) using the provided routine */
    seq_t sqrtv = floor_sqrt(x);
    idx_t limit = (idx_t)sqrtv;          /* limit fits in idx_t */

    seq_t remain = x;

    for (idx_t d = 3; d <= limit; d += 2) {
        if ((seq_t)d * (seq_t)d > remain)
            break;
        if (remain % (seq_t)d == 0) {
            unsigned int exp = 0;
            while (remain % (seq_t)d == 0) {
                remain /= (seq_t)d;
                ++exp;
            }
            if (((d & 3U) == 3U) && (exp & 1U))
                return true;            /* condition violated */
        }
    }

    /* If any factor > sqrt remains, it must be prime. */
    if (remain > 1) {
        if ((seq_t)remain % 4U == 3U)
            return true;
    }
    return false;                         /* can be written as sum of two squares */
}

/* Public entry point: a(n) returns the nth odd integer that is not the sum
   of two squares.  The function works for any order of calls and is O(n^2)
   in the worst case, fast enough for n ≤ 10000. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                      /* undefined for n < 1, return 0 */

    idx_t count = 0;
    seq_t candidate = 3;                /* first odd > 1 */

    while (true) {
        if (not_sum_two_squares_odd(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 2;                 /* next odd integer */
    }
}
