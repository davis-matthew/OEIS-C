/*
 * A219109  The smallest k such that prime(k) == -1 (mod n).
 * https://oeis.org/A219109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 42, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A219109: the smallest k such that prime(k) ≡ -1 (mod n). */
/* The program recomputes the result independently for each call; it
   does not use caching or a global list of primes.  The algorithm
   simply iterates over the primes until it finds a prime that is
   congruent to n-1 modulo n.  This works for all n ≥ 1.  */

#include <stdint.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* n is 1‑based.  a(1) = 1 by definition.  */
    if (n == 0) return 0;                /* outside the allowed range */

    idx_t k = 1;                         /* index of the current prime */
    uint64_t cand = 2;                   /* the current prime candidate */

    while (1) {
        seq_t prime = (seq_t)cand;       /* cast to seq_t for arithmetic */

        if ((prime % (seq_t)n) == (seq_t)(n - 1))
            return (seq_t)k;            /* found the smallest such k */

        /* advance to the next prime */
        k++;
        if (cand == 2)
            cand = 3;
        else {
            cand += 2;                  /* skip even numbers */
            while (!is_prime_big((seq_t)cand))
                cand += 2;
        }
    }
}
