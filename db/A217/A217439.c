/*
 * A217439  Primes or negative values of primes of the form 8*n^2 - 298*n +
 *          2113 for n >= 0.
 * https://oeis.org/A217439
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, less
 * Formula:   not stated
 * Range:     i8 none, i16 n <= 48, i32 n <= 48, i64 n <= 48, i128 n <= 48,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n‑th term of OEIS A217439.
   The sequence consists of the values 8*m² – 298*m + 2113 (m ≥ 0)
   whose absolute value is prime.  The terms are returned in order of
   increasing m.  a(1) corresponds to m = 0. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t m = 0; ; ++m)
    {
        seq_t m2 = (seq_t)m * (seq_t)m;
        seq_t v  = (seq_t)8 * m2 - (seq_t)298 * (seq_t)m + 2113;
        seq_t absv = v >= 0 ? v : -v;
        if (is_prime_big(absv))
        {
            ++count;
            if (count == n)
                return v;
        }
    }
}
