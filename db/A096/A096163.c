/*
 * A096163  Primes p of the form qrs + 1 where q, r and s are distinct primes.
 * https://oeis.org/A096163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   count_distinct_primes, count_prime_factors, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Nth prime p with p‑1 = q·r·s where q, r, s are distinct primes
   (OEIS A096163).  The function searches primes sequentially,
   until the required number of suitable primes has been found. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/next_prime.h"              /* uint64_t next_prime(uint64_t start); */
#include "../utils/count_distinct_primes.h"   /* int count_distinct_primes(uint64_t n); */
#include "../utils/count_prime_factors.h"     /* seq_t count_prime_factors(seq_t n); */

/*--- Public function ----------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Offset is 1 – undefined for n = 0 */

    idx_t found = 0;               /* how many suitable primes seen */
    uint64_t candidate = 2;        /* start at the first prime */

    while (found < n) {
        /* get next prime (includes candidate if it is prime) */
        candidate = next_prime(candidate);

        uint64_t m = candidate - 1;                  /* candidate‑1 */
        if (count_distinct_primes(m) != 3) {          /* need exactly 3 distinct primes */
            ++candidate;   /* skip to next odd (next_prime will handle this) */
            continue;
        }
        /* count_prime_factors returns the total number of prime factors
           (with multiplicity).  We require exactly 3 and all distinct.    */
        if (count_prime_factors((seq_t)m) != 3) {
            ++candidate;
            continue;
        }

        ++found;
        if (found == n) {
            return (seq_t)candidate;                  /* found the nth term */
        }
    }

    /* The loop above is guaranteed to terminate for all n >= 1 */
    return 0;   /* unreachable, but keeps the compiler happy */
}
