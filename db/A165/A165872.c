/*
 * A165872  Totally multiplicative sequence with a(p^e) = (-2)^e.
 * https://oeis.org/A165872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) = (-2)^bigomega(n)
 * Range:     i8 n <= 255, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    /* Number of prime factors of n counted with multiplicity */
    seq_t exp = bigomega(n);
    seq_t res = 1;
    seq_t base = -2;

    for (idx_t i = 0; i < (idx_t)exp; ++i)
        res *= base;

    return res;
}
