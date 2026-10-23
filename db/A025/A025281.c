/*
 * A025281  a(n) = sopfr(n!), where sopfr = A001414 is the integer log.
 * https://oeis.org/A025281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A001414(k) (sopfr of factorial n!)
 * Range:     u8 n <= 28, u16 n <= 652, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=0, a(1)=0, a(n) = a(n-1) + sopfr(n) for n>1. */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle offset and trivial cases */
    if (n <= 1) return 0;

    /* Work with unsigned size_t for array bounds */
    size_t N = (size_t)n + 1;                /* numbers 0..n inclusive                 */
    uint64_t *spf = (uint64_t *)malloc(N * sizeof(uint64_t));
    if (!spf) exit(EXIT_FAILURE);
    size_t *primes = (size_t *)malloc(N * sizeof(size_t));
    if (!primes) { free(spf); exit(EXIT_FAILURE); }

    /* Linear sieve that stores the smallest prime factor of each i */
    size_t primes_cnt = 0;
    memset(spf, 0, N * sizeof(uint64_t));    /* ensures zero for i >= 2 until set */
    for (size_t i = 2; i < N; ++i) {
        if (spf[i] == 0) {                  /* i is prime */
            spf[i] = i;
            primes[primes_cnt++] = i;
        }
        for (size_t j = 0; j < primes_cnt; ++j) {
            size_t p = primes[j];
            uint64_t ip = i * p;
            if (p > spf[i] || ip >= N) break;
            spf[ip] = p;
        }
    }

    /* Compute sopfr values and accumulate the sum */
    seq_t sum = 0;
    seq_t *sopfr = (seq_t *)malloc(N * sizeof(seq_t));
    if (!sopfr) { free(spf); free(primes); exit(EXIT_FAILURE); }

    sopfr[0] = sopfr[1] = 0;
    for (size_t i = 2; i < N; ++i) {
        sopfr[i] = sopfr[i / spf[i]] + (seq_t)spf[i];
        sum += sopfr[i];
    }

    /* Clean up */
    free(sopfr);
    free(spf);
    free(primes);

    return sum;
}
