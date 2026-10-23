/*
 * A135683  Duplicate of A005451.
 * https://oeis.org/A135683
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 1 if n is prime, otherwise a(n) = n. a(1) = 1
 * Range:     i8 n <= 127, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    if (is_prime(n))
        return (seq_t)1;
    return (seq_t)n;
}
