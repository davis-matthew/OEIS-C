/*
 * A175933  Number of ways of writing n=p+k with p a prime number and k a
 *          primorial number.
 * https://oeis.org/A175933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered representations n = p + q, where p is
 *            a prime and q is a primorial (product of the first k primes, q ≥
 *            1)
 * Range:     u8 n <= 3666, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;  /* no primes ≤ 1 */

    uint64_t *primes = NULL;
    size_t primes_cnt = 0;
    /* build all primes up to n */
    build_primes_upto(n, &primes, &primes_cnt);

    seq_t count = 0;
    seq_t prim = 1;            /* primorial of 0 primes */
    if (n > prim && is_prime((idx_t)(n - prim))) count++;

    /* generate subsequent primorials using the prime list */
    for (size_t i = 0; i < primes_cnt; ++i) {
        if (prim > n / (seq_t)primes[i]) break; /* next product would exceed n */
        prim = prim * (seq_t)primes[i];
        if (prim > n) break;
        if (is_prime((idx_t)(n - prim))) count++;
    }

    free(primes);
    return count;
}
