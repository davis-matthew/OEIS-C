/*
 * A106743  a(n) = -1 iff n is prime, a(n) = 1 iff n is not squarefree,
 *          otherwise (n is nonprime and squarefree) a(n) = 0.
 * https://oeis.org/A106743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -1 if n is prime, 1 if n is not squarefree (has a squared
 *            prime factor), 0 otherwise (composite and squarefree)
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Simple trial‑division primality test, O(√n). */
static bool is_prime(idx_t n)
{
    if (n < 2) return false;
    if (n == 2) return true;
    if ((n & 1) == 0) return false;
    for (idx_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) return false;
    }
    return true;
}

/* Test whether n is squarefree.
   Returns false if some prime factor occurs with exponent ≥2. */
static bool is_squarefree(idx_t n)
{
    if (n <= 1) return true;
    for (idx_t p = 2; p <= n / p; ++p) {
        if (n % p == 0) {
            n /= p;            /* remove exponent 1 */
            if (n % p == 0)    /* exponent ≥2 found */
                return false;
        }
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;          /* offset 1 */
    if (is_prime(n)) return (seq_t)-1;    /* prime */
    if (!is_squarefree(n)) return (seq_t)1;/* composite & not squarefree */
    return (seq_t)0;                      /* composite & squarefree */
}
