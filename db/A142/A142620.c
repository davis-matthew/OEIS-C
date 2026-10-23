/*
 * A142620  Primes congruent to 27 mod 55.
 * https://oeis.org/A142620
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   the sequence consists of primes that are congruent to 27 modulo
 *            55. a(n) is the n-th such prime (a(1)=137)
 * Range:     u8 n <= 1, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t x) */

/* Return the n-th prime ≡ 27 (mod 55).  For n == 0 (outside the
   defined range) a zero value is returned.  The function works for
   arbitrary seq_t, idx_t types defined by the build. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                    /* valid guard for out‑of‑range calls */

    idx_t count = 0;                         /* number of qualifying primes found */
    idx_t k = 0;                             /* multiplier for 55*k + 27 */

    for (;;) {
        idx_t candidate = 55 * k + 27;       /* candidate number to test */

        if (is_prime(candidate)) {
            ++count;                        /* we found a qualifying prime */
            if (count == n)
                return (seq_t)candidate;     /* return it, cast to seq_t */
        }
        ++k;
    }
}
