/*
 * A186300  (A007521(n)+1)/2.
 * https://oeis.org/A186300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p₅(n) + 1)/2, where p₅(n) is the n-th prime ≡ 5 (mod 8)
 * Range:     u8 n <= 14, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   estimate_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/estimate_bound.h"

/* Compute the sequence term a(n). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but safe guard */

    /* Static cache of computed primes */
    static uint64_t *prime5   = NULL; /* array of primes ≡ 5 (mod 8) */
    static size_t  prime_cnt = 0;     /* number of primes stored */
    static idx_t   max_n     = 0;     /* largest n processed so far */

    if (n <= max_n && prime_cnt >= (size_t)n) {
        uint64_t p = prime5[n-1];
        seq_t res = (seq_t)p + 1;
        res /= 2;
        return res;
    }

    /* Need to recompute cache up to n */
    max_n = n;
    size_t bound = estimate_bound(n);
    /* The following may be repeated if the bound proved too small. */
    while (1) {
        /* Allocate % sieve array */
        char *is_prime = (char*)malloc(bound + 1);
        if (!is_prime) return 0;          /* out of memory */
        memset(is_prime, 1, bound + 1);
        if (bound >= 0) is_prime[0] = 0;
        if (bound >= 1) is_prime[1] = 0;

        /* Eratosthenes sieve */
        for (size_t i = 2; i * i <= bound; ++i) {
            if (is_prime[i]) {
                for (size_t j = i * i; j <= bound; j += i)
                    is_prime[j] = 0;
            }
        }

        /* Collect primes ≡ 5 (mod 8) */
        free(prime5);
        prime5 = (uint64_t*)malloc(n * sizeof(uint64_t));
        if (!prime5) { free(is_prime); return 0; }
        prime_cnt = 0;
        for (size_t i = 5; i <= bound && prime_cnt < n; ++i) {
            if (is_prime[i] && (i % 8 == 5))
                prime5[prime_cnt++] = i;
        }

        free(is_prime);

        if (prime_cnt >= (size_t)n) break;   /* we have enough primes */
        bound *= 2;                          /* increase bound and retry */
    }

    uint64_t p = prime5[n-1];
    seq_t res = (seq_t)p + 1;
    res /= 2;
    return res;
}
