/*
 * A142110  Primes congruent to 31 mod 36.
 * https://oeis.org/A142110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 31 (mod 36)
 * Range:     u8 n <= 5, u16 n <= 412, u32 n <= 452, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function simply generates successive candidates of the form
   31 + 36*k, tests them for primality using the provided
   is_prime_big() routine, and returns the n-th such prime.
   No caching or dynamic allocation of seq_t objects is performed,
   so the implementation works correctly whether seq_t is an
   unsigned integer type or an arbitrary‑precision integer class. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n); */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* offset starts at 1 – undefined for n==0 */

    seq_t candidate = (seq_t)31;    /* first candidate (31 itself) */
    idx_t   count   = 1;            /* primes found so far, starting with 31 */

    while (count < n) {
        candidate += (seq_t)36;    /* next candidate of the form 31 + 36*k */

        if (is_prime_big(candidate))
            ++count;               /* found one more prime of the desired form */
    }

    return candidate;
}
