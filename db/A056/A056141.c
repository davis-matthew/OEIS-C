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
 * Helpers:   isPrime, nextPrime, primeCeiling, primeFloor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrime.h"
#include "../utils/nextPrime.h"
#include "../utils/primeCeiling.h"
#include "../utils/primeFloor.h"

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
