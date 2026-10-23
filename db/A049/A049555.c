/*
 * A049555  Primes p such that x^23 = 2 has a solution mod p.
 * https://oeis.org/A049555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes_upto, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A049555
   a(n) = the n-th prime p such that the congruence x^23 ≡ 2 (mod p) has a solution.
   This is equivalent to p=2 or p≠2 and 2^((p-1)/gcd(23,p-1)) ≡ 1 (mod p).
   Since 23 is prime, gcd(23,p-1) is 1 (always satisfies) or 23.  
   Hence all primes with p≠1 (mod 23) are included, and for p≡1 (mod23) we test
   the extra condition 2^((p-1)/23) ≡ 1 (mod p).
*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    static uint64_t *prime_list = NULL;
    static size_t prime_count = 0;

    idx_t idx = 0;                 /* the nth element to return (1‑based) */
    idx_t found = 0;               /* how many satisfying primes found so far */

    /* Start with a small limit and double until enough primes are found */
    idx_t limit = 100;
    while (true) {
        /* Build list of all primes <= limit */
        size_t cnt = 0;
        uint64_t *list = NULL;
        build_primes_upto(limit, &list, &cnt);
        prime_list = list;
        prime_count = cnt;

        found = 0;
        for (idx_t i = 0; i < prime_count; ++i) {
            uint64_t p = prime_list[i];
            if (p == 2) {
                /* 0^23 ≡ 2 (mod 2) holds */
                found++;
                if (found == n) return (seq_t)p;
                continue;
            }
            /* p is odd prime > 2 */
            if ((p - 1) % 23 != 0) {
                /* gcd(23,p-1)=1 -> always solvable */
                found++;
                if (found == n) return (seq_t)p;
                continue;
            }
            /* p ≡ 1 (mod 23). Need to check 2^((p-1)/23) ≡ 1 (mod p). */
            uint64_t exp = (p - 1) / 23;
            seq_t m = mod_pow((seq_t)2, exp, (seq_t)p);
            if (m == (seq_t)1) {
                found++;
                if (found == n) return (seq_t)p;
            }
        }

        /* Not enough found; increase limit and repeat */
        free(prime_list); /* free memory from build_primes_upto */
        prime_list = NULL;
        prime_count = 0;
        limit *= 2;
    }
}
