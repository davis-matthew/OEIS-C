/*
 * A156709  For all numbers k(n) congruent to -1 or +1 (mod 6) starting with
 *          k(n) = {5,7,11,13,...}, a(k(n)) is incremented by the congruence
 *          (mod 6) if k(n) is prime and by 0 if k(n) is composite.
 * https://oeis.org/A156709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 9469, i16 n <= 9469, i32 n <= 9469, i64 n <= 9469,
 *            i128 n <= 9469, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = cumulative sum of ±1 signs for primes congruent to ±1 (mod 6) among
   the first n numbers 5, 7, 11, 13, 17, 19, …  (sign +1 for ≡1 (mod 6),
   sign -1 for ≡5 (mod 6)).  A composite contributes 0. */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t cum = 0;          /* running total of the signs                */
    idx_t  cnt = 0;         /* how many k‐values have been processed     */
    idx_t  k   = 5;         /* the first number ≡ ±1 (mod 6)             */
    bool   step2 = true;    /* first step is +2, then alternate +4      */

    while (cnt < n)
    {
        if (is_prime(k))
            cum += (k % 6 == 1) ? 1 : -1;

        cnt++;

        /* advance to the next number ≡ ±1 (mod 6) */
        k += step2 ? 2 : 4;
        step2 = !step2;
    }
    return cum;
}
