/*
 * A174092  Primes p = -83 + n * 30 (n = 0, 1, ..., 8) forming "optimal" 3 x 3
 *          Magic Prime Square.
 * https://oeis.org/A174092
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, fini, uned, sign
 * Formula:   a(n) = the nth prime of the form -83 + 30*k (k ≥ 0)
 * Range:     i8 n <= 8, i16 n <= 9, i32 n <= 9, i64 n <= 9, i128 n <= 9,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t) */

seq_t a(idx_t n)
{
    idx_t i = 0;           /* current k in -83 + 30*k          */
    idx_t count = 0;       /* number of primes found so far  */
    long long m = -83;     /* value of -83 + 30*i              */

    /* Iterate until n primes are found or the value no longer fits in seq_t */
    while (1) {
        /* test if m fits into seq_t */
        seq_t sm = (seq_t)m;
        if ((long long)sm != m)                 /* overflow → stop */
            break;

        /* absolute value for primality test */
        long long abs_m = (m < 0) ? -m : m;

        if (is_prime((idx_t)abs_m)) {           /* m (or –m) is a prime */
            ++count;
            if (count == n)                     /* found the nth term   */
                return sm;
        }

        /* next candidate: -83 + 30*(i+1) */
        ++i;
        m = -83 + 30 * (long long)i;
    }

    /* n is too large for the given seq_t range – not expected in tests */
    return 0;
}
