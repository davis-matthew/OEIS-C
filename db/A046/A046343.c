/*
 * A046343  Sum of the prime factors of the composite numbers (counted with
 *          multiplicity).
 * https://oeis.org/A046343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the prime factors (with multiplicity) of the nth
 *            composite number * (i.e., a(n) = A001414(A002808(n)))
 * Range:     u8 n <= 415, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* Helper functions are available from the build, but we are not using them here.
 * We perform a small linear sieve each call to keep the implementation simple
 * and portable across C11 and C++17 builds. */

seq_t a(idx_t n)
{
    /* Ensure valid n (offset 1).  For n == 0, return 0 (undefined). */
    if (n == 0) return 0;

    /* Choose a sieve upper bound that is more than enough to contain n composites.
     * For safety we take three times n plus a small constant. */
    idx_t limit = n * 3 + 100;

    /* Allocate array for smallest prime factors (spf). */
    size_t spf_size = (size_t)(limit + 1);
    uint32_t *spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    if (!spf) return 0;                 /* Allocation failure – return 0 */
    memset(spf, 0, spf_size * sizeof(uint32_t));

    /* Linear sieve for spf.  We also keep a list of primes. */
    uint32_t *primes = NULL;
    size_t primes_cap = 64;
    primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) { free(spf); return 0; }
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= limit; ++i) {
        if (spf[i] == 0) {                /* i is prime */
            spf[i] = (uint32_t)i;
            if (primes_cnt == primes_cap) {      /* grow primes array */
                primes_cap <<= 1;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(spf); free(primes); return 0; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
        for (size_t j = 0; j < primes_cnt && primes[j] <= spf[i] &&
             (idx_t)i * primes[j] <= limit; ++j) {
            spf[(idx_t)i * primes[j]] = primes[j];
        }
    }

    /* Now scan numbers >= 2 to find the nth composite and compute its prime‑factor sum. */
    idx_t comp = 0;
    seq_t result = 0;
    for (idx_t i = 2; comp < n && i <= limit; ++i) {
        if (spf[i] != i) {           /* i is composite */
            ++comp;
            if (comp == n) {
                /* Compute the sum of prime factors of i. */
                idx_t temp = i;
                seq_t sum = 0;
                while (temp > 1) {
                    sum += (seq_t)spf[temp];
                    temp /= spf[temp];
                }
                result = sum;
            }
        }
    }

    /* Clean up */
    free(spf);
    free(primes);

    return result;
}
