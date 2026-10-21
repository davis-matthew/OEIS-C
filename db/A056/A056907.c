/*
 * A056907  Numbers k such that 36*k^2 + 12*k + 5 is prime (sorted by absolute
 *          values with negatives before positives).
 * https://oeis.org/A056907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Numbers k such that 36*k^2 + 12*k + 5 is prime (sorted by
 *            absolute values with negatives before positives)
 * Range:     i8 n <= 2, i16 n <= 20, i32 n <= 23, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 0 is always a member because 36*0^2 + 12*0 + 5 = 5, which is prime. */
    if (n == 0)
        return 0;

    /* count holds the 0‑based index of the next term to return. */
    size_t count = 1;           /* we have already counted the zero term. */
    idx_t  m = 1;               /* |k| starting from 1. */

    while (true) {
        /* k = -m */
        seq_t k   = -(seq_t)m;
        seq_t val = (seq_t)36 * k * k + (seq_t)12 * k + (seq_t)5;
        if (is_prime_big(val)) {
            if (count == n)
                return k;
            count++;
        }

        /* k = +m */
        k   = (seq_t)m;
        val = (seq_t)36 * k * k + (seq_t)12 * k + (seq_t)5;
        if (is_prime_big(val)) {
            if (count == n)
                return k;
            count++;
        }

        m++;
    }
}
