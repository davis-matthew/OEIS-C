/*
 * A071195  Final prime in sequence of primes starting with prime(n) and
 *          having prime sum (see A071194), or -1 if no such sequence exists.
 * https://oeis.org/A071195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 2682, u32 n <= 2682, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the final prime in the sequence of consecutive primes
   starting with prime(n) that has a prime sum.  Returns -1 (as a
   large unsigned value) if no such sequence exists. */

#include <stdlib.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  If called with an index less than 1 the behaviour
       is unspecified, but the OEIS offset is 1 so this is fine. */

    /* Sum of successive primes starting at prime(n). */
    seq_t sum = 0;
    /* Result is the last prime added when the partial sum becomes
       prime.  Initialise to -1, which is the maximum unsigned value
       for all supported seq_t types. */
    seq_t result = (seq_t)-1;

    for (idx_t len = 1;; ++len) {
        /* prime(n + len - 1) is the len‑th prime starting from prime(n) */
        seq_t p = (seq_t)get_prime(n + len - 1);
        sum += p;

        if (len >= 2 && is_prime_big(sum)) {
            result = p;
            break;
        }

        /* Prevent an infinite loop in pathological cases.  The
           1 000 000 bound is safely above the necessary length for all
           tested values of n. */
        if (len > 1000000) {
            break;
        }
    }

    return result;
}
