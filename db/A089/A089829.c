/*
 * A089829  Triangle T(p,k) read by rows, where p runs through the odd primes
 *          and 2 <= k <= p-1. T(p,k) = 1 if the reverse of the base-k
 *          expansion of p is a prime, otherwise 0.
 * https://oeis.org/A089829
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  base, easy, nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   build_primes_upto, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) of OEIS A089829.
   For n ≥ 3 locate the odd prime p and the index k
   such that this term lies in row p (k ranges 2..p-1).
   The term is 1 if the reverse of the base‑k representation of p
   (interpreted again in base k) is a prime, otherwise 0. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n < 3) return (seq_t)0;          /* offset is 3; behaviour below is irrelevant */

    idx_t n_offset = n - 3;              /* zero‑based index in the sequence */

    /* Cache of odd primes */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;         /* number of primes in the array */
    static uint64_t prime_limit = 100;   /* current upper bound used for building primes */

    /* Helper to rebuild the prime table up to a given limit */
    while (true) {
        /* Build or rebuild prime table if necessary */
        if (!primes) {
            build_primes_upto(prime_limit, &primes, &prime_cnt);
        }

        /* Scan through primes to locate the desired row */
        idx_t cumulative = 0;
        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p = primes[i];
            if (p < 3) continue;          /* skip the prime 2 */

            idx_t block = p - 2;          /* number of terms in this row */
            if (cumulative + block > n_offset) {
                uint64_t k = 2 + (n_offset - cumulative);

                /* Compute reverse of base‑k representation of p */
                seq_t rev = 0;
                uint64_t tmp = p;
                while (tmp > 0) {
                    uint64_t d = tmp % k;
                    tmp /= k;
                    rev = rev * (seq_t)k + (seq_t)d;
                }

                /* Test primality of the reversed value */
                bool is_rev_prime = is_prime_big(rev);
                return (seq_t)is_rev_prime;
            }
            cumulative += block;
        }

        /* Not found yet: extend the prime table and retry */
        prime_limit *= 2;
        free(primes);
        primes = NULL;
        prime_cnt = 0;
    }
}
