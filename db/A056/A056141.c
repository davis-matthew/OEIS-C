/*
 * A056141  a(n) = primefloor(n)*primeceiling(n) -
 *          previousprime(n)*nextprime(n).
 * https://oeis.org/A056141
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = primefloor(n)*primeceiling(n) -
 *            previousprime(n)*nextprime(n)
 * Range:     i8 n <= 28, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 79 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

/* Smallest prime > n (nextprime) */
static seq_t nextPrime(idx_t n)
{
    for (idx_t k = n+1; ; ++k)
    {
        if (is_prime(k))           return (seq_t)k;
    }
}

/* Smallest prime >= n (primeceiling) */
static seq_t primeCeiling(idx_t n)
{
    for (idx_t k = n; ; ++k)
    {
        if (is_prime(k))           return (seq_t)k;
    }
}

/* Largest prime <= n (primefloor) */
static seq_t primeFloor(idx_t n)
{
    if (n < 2)                    return 0;
    for (idx_t k = n; k >= 2; --k)
    {
        if (is_prime(k))           return (seq_t)k;
        if (k == 2)                break;   /* avoid underflow */
    }
    return 0;
}

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 3; below that the result is
       irrelevant for the tests. */
    if (n < 3) return 0;

    seq_t p_floor = primeFloor(n);
    seq_t p_ceiling = primeCeiling(n);
    seq_t p_prev   = primeFloor(n-1);   /* previous prime before n */
    seq_t p_next   = nextPrime(n);      /* next prime after n */

    return (p_floor * p_ceiling) - (p_prev * p_next);
}
