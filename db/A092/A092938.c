/*
 * A092938  a(n) = least prime p such that 2*prime(n) - p is prime.
 * https://oeis.org/A092938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that 2*prime(n) - p is prime. Search
 *            over all primes p ≤ prime(n); the first one for which 2*prime(n)
 *            - p is also prime is returned
 * Range:     u8 n <= 31, u16 n <= 3367, u32 n <= 5151, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                       /* undefined, safe */

    /* Fetch the n‑th prime. */
    seq_t r = (seq_t)get_prime(n);                     /* r = prime(n) */

    /* Cache all primes up to the current r for reuse across calls. */
    static uint64_t *prime_list = NULL;                /* primes ≤ prime_limit */
    static size_t   prime_count = 0;                   /* number of primes cached */
    static idx_t    prime_limit  = 0;                  /* current cached limit */

    if (prime_list == NULL || (idx_t)r > prime_limit) {
        uint64_t *list = NULL;
        size_t    cnt  = 0;
        build_primes_upto((idx_t)r, &list, &cnt);
        if (prime_list) free(prime_list);
        prime_list  = list;
        prime_count = cnt;
        prime_limit = (idx_t)r;
    }

    /* Search for the smallest prime p such that 2*r - p is prime. */
    seq_t two_r = r + r;                               /* 2 * r */
    for (size_t i = 0; i < prime_count; ++i) {
        seq_t p = (seq_t)prime_list[i];
        seq_t q = two_r - p;                           /* q = 2*r - p */
        if (is_prime_big(q))
            return p;                                  /* found the least p */
    }

    return (seq_t)0; /* should not be reached for valid n ≥ 1 */
}
