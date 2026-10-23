/*
 * A142397  Primes congruent to 46 mod 47.
 * https://oeis.org/A142397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 146, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the nth prime congruent to 46 (mod 47). */
/* a(1) = 281, a(2) = 563, …  */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* External linkage as requested. */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* offset starts at 1 */

    idx_t k = 0;          /* multiplier for 47: candidate = 47*k + 46 */
    idx_t found = 0;      /* number of primes found so far */

    while (1) {
        k++;                                    /* next k */
        idx_t candidate = (idx_t)47 * k + 46;    /* candidate prime */
        if (is_prime(candidate)) {               /* use supplied helper */
            found++;
            if (found == n)
                return (seq_t)candidate;         /* cast to the result type */
        }
    }
}
