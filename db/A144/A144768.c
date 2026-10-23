/*
 * A144768  a(n) = n! - n^9.
 * https://oeis.org/A144768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n! - n^9
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 10, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   big_pow, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factorial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t f = factorial(n);
    seq_t p = big_pow((seq_t)n, (seq_t)9);
    return f - p;
}
