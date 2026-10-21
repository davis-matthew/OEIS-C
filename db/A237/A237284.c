/*
 * A237284  Number of ordered ways to write 2*n = p + q with p, q and
 *          A000720(p) all prime.
 * https://oeis.org/A237284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(p,q) primes | p+q = 2*n and pi(p) is prime} (pi(p) =
 *            number of primes ≤ p = A000720(p) )
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    idx_t limit = n * 2;                     /* 2*n */
    size_t sz = (size_t)limit + 1;           /* array size for sieve */

    /* primality array for numbers up to limit (0/1) */
    unsigned char *is_prime = (unsigned char*)calloc(sz, sizeof(unsigned char));
    if (!is_prime) return 0;
    /* 0 and 1 are not prime */
    is_prime[0] = 0;
    is_prime[1] = 0;
    /* mark all others initially as prime */
    for (size_t i = 2; i < sz; ++i) is_prime[i] = 1;

    /* Eratosthenes sieve up to sqrt(limit) */
    for (idx_t i = 2; (idx_t)i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* compute prime index for each number (0 if composite) */
    uint32_t *prime_index = (uint32_t*)calloc(sz, sizeof(uint32_t));
    if (!prime_index) {
        free(is_prime);
        return 0;
    }
    uint32_t count = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) {
            ++count;
            prime_index[i] = count;   /* 1-based index */
        }
    }

    /* count ordered pairs (p,q)  */
    seq_t res = 0;
    for (idx_t i = 2; i <= limit; ++i) {
        if (!is_prime[i]) continue;          /* p must be prime */
        uint32_t pi = prime_index[i];
        if (pi == 0) continue;              /* safety, should not happen */
        if (!is_prime[pi]) continue;        /* pi(p) must be prime          */
        idx_t q = limit - i;                /* q = 2*n - p */
        if (q < 2 || q > limit) continue;  /* q must be within range */
        if (is_prime[q])                   /* q must be prime             */
            ++res;                          /* ordered pair (p,q) counted   */
    }

    free(is_prime);
    free(prime_index);
    return res;
}
