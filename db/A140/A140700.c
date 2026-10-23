/*
 * A140700  a(n) = n*A069158(n).
 * https://oeis.org/A140700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 128, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = n * f(n)
    where
        f(n) = 0                    if n has a squared prime factor;
        f(n) = -1   if n is a prime;
        f(n) = +1   if n is a square‑free composite.
    Hence a(1)=1, a(p)=-p for primes p, a(n)=+n for square‑free composites
    and 0 for numbers with a squared factor.
*/

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n == 1) return 1;                 /* offset = 1 */

    idx_t t = n;
    int prime_cnt = 0;                    /* number of distinct prime factors */

    for (idx_t p = 2; p <= t / p; ++p) { /* trial division up to sqrt(t) */
        if (t % p == 0) {                /* p is a prime factor */
            prime_cnt++;
            int mult = 0;                 /* multiplicity of p */
            while (t % p == 0) {
                t /= p;
                mult++;
                if (mult > 1) {           /* squared factor found */
                    return 0;
                }
            }
        }
    }
    if (t > 1) {                          /* remaining prime factor > sqrt(original n) */
        prime_cnt++;
    }

    /* n is square‑free.  For n>1 the prime_cnt is at least 1. */
    if (prime_cnt == 1) {                 /* n is a prime */
        return -(seq_t)n;
    }
    /* composite square‑free */
    return (seq_t)n;
}
