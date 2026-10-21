/*
 * A088004  "Truncated Mertens function": values of -1 at primes are left out,
 *          that is, summatory Moebius when argument runs through nonprimes.
 * https://oeis.org/A088004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002321(n) + A000720(n) (Mertens function + prime count)
 * Range:     u8 n <= 1712, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Allocate temporary arrays of size n+1 */
    size_t N = (size_t)n;
    uint8_t *comp = (uint8_t *)calloc(N + 1, sizeof(uint8_t));
    if (!comp) return 0;

    uint32_t *pr = (uint32_t *)malloc((N + 1) * sizeof(uint32_t));
    if (!pr) { free(comp); return 0; }

    int8_t *mu = (int8_t *)malloc((N + 1) * sizeof(int8_t));
    if (!mu) { free(comp); free(pr); return 0; }

    /* Initialization */
    mu[1] = 1;
    seq_t sum = 1;           /* μ(1) = 1 */
    seq_t prime_cnt = 0;
    size_t pcnt = 0;         /* number of primes found */

    /* Linear sieve: compute μ(k) for 2..n and count primes */
    for (idx_t i = 2; i <= n; ++i) {
        if (!comp[i]) {          /* i is prime */
            pr[pcnt++] = (uint32_t)i;
            mu[i] = -1;
            prime_cnt += 1;
        }
        for (size_t j = 0; j < pcnt; ++j) {
            uint32_t p = pr[j];
            idx_t prod = i * p;
            if (prod > n) break;
            comp[prod] = 1;
            if (i % p == 0) {
                mu[prod] = 0;
                break;
            } else {
                mu[prod] = -mu[i];
            }
        }
        sum += (seq_t)mu[i];
    }

    /* Clean up */
    free(comp);
    free(pr);
    free(mu);

    return sum + prime_cnt;
}
