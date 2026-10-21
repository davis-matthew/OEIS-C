/*
 * A065345  a(n) = binomial(2*n,n) mod ((n+1)*(n+2)*(n+3)).
 * https://oeis.org/A065345
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(2*n,n) mod ((n+1)*(n+2)*(n+3))
 * Range:     u8 n <= 5, u16 n <= 51, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* sequence is defined for n>=1, but guard */

    idx_t limit = 2 * n;

    /* Cached primes up to the largest needed limit */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static idx_t current_limit = 0;

    if (limit > current_limit) {
        /* Rebuild the prime list for the new limit */
        free(primes);
        current_limit = limit;

        /* Simple sieve to find all primes <= limit */
        size_t sz = (size_t)(limit + 1);
        uint8_t *is_comp = (uint8_t*)calloc(sz, 1);
        if (is_comp == NULL) abort();        /* allocation failure */

        /* First pass: count primes */
        size_t cnt = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[i]) {
                ++cnt;
                if ((uint64_t)i * i <= limit) {
                    for (idx_t j = i * i; j <= limit; j += i)
                        is_comp[j] = 1;
                }
            }
        }

        /* Allocate and second pass: store primes */
        primes = (idx_t*)malloc(cnt * sizeof(idx_t));
        if (primes == NULL) abort();        /* allocation failure */
        size_t pc = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (!is_comp[i]) primes[pc++] = i;
        }
        prime_cnt = cnt;
        free(is_comp);
    }

    /* Compute modulus m = (n+1)*(n+2)*(n+3) */
    uint64_t m = (uint64_t)(n + 1) * (n + 2) * (n + 3);
    uint64_t res = 1ULL;

    for (size_t pi = 0; pi < prime_cnt; ++pi) {
        idx_t p = primes[pi];
        /* Exponent of prime p in binomial(2n,n) = sum_{k>=1} (floor(2n/p^k) - 2*floor(n/p^k)) */
        idx_t e = 0;
        idx_t power = p;
        while (power <= 2 * n) {
            e += (2 * n) / power;
            e -= 2 * (n / power);
            /* Prevent overflow of power on next iteration */
            if (power > (UINT64_MAX / p)) break;
            power *= p;
        }

        /* Multiply result by p^e modulo m */
        for (idx_t k = 0; k < e; ++k) {
            res = (unsigned __int128)res * p % m;
        }
    }

    return (seq_t)res;
}
