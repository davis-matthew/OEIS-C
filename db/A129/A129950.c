/*
 * A129950  Indicator function of twin primes: 1 if n is a twin prime member,
 *          0 if not prime, -1 else (isolated prime or 2).
 * https://oeis.org/A129950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 2*A164292(n) - A010051(n)
 * Range:     i8 n <= 9522, i16 n <= 9522, i32 n <= 9522, i64 n <= 9522,
 *            i128 n <= 9522, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 1 if n is a twin‑prime member, 0 if n is not prime, -1 if n is an isolated prime (including 2). */
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (!is_prime(n)) return 0;

    int twin = 0;
    if (n >= 3 && is_prime(n - 2)) twin = 1;
    if (is_prime(n + 2)) twin = 1;

    return (seq_t)(twin ? 1 : -1);
}
