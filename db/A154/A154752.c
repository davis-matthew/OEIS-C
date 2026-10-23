/*
 * A154752  Least prime p (or 0) such that n = p + T, where T is a triangular
 *          number (A000217), or -1 if there is no such representation.
 * https://oeis.org/A154752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = least prime p (or 0) such that n = p + T, where T is a
 *            triangular number (A000217). If no such prime exists, return -1
 * Range:     i8 n <= 138, i16 n <= 8268, i32 n <= 8268, i64 n <= 8268,
 *            i128 n <= 8268, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* sentinel for “no prime found yet” */
    seq_t minp = (seq_t)-1;

    /* Enumerate triangular numbers t = k*(k+1)/2  (k >= 0) */
    for (idx_t k = 0; ; ++k)
    {
        seq_t t = (seq_t)k * (k + 1) / 2;          /* triangular number */
        if (t > (seq_t)n) break;                  /* further t are too large */

        seq_t p = (seq_t)n - t;                   /* candidate prime */
        if (p == 0) return (seq_t)0;              /* n itself is triangular */

        if (p > 0)                                 /* p positive ⇒ fits in idx_t */
        {
            idx_t p_idx = (idx_t)p;
            if (is_prime(p_idx))
            {
                if (minp == (seq_t)-1 || p_idx < (idx_t)minp)
                    minp = (seq_t)p_idx;
            }
        }
    }
    return minp;                                   /* -1 if none found */
}
