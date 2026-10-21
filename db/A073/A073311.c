/*
 * A073311  Number of squarefree numbers in the reduced residue system of n.
 * https://oeis.org/A073311
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n, gcd(n,k)=1} mu(k)^2 where mu(k) is the
 *            Möbius function (mu(k)=0 if k is not squarefree)
 * Range:     u8 n <= 276, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* return 0 for the unused index 0 (offset is 1) */
    if (n == 0) return (seq_t)0;

    /* allocate array for Möbius values: mu[i] in { -1, 0, 1 } */
    int *mu = (int *)calloc((size_t)n + 1, sizeof(int));
    if (!mu) return (seq_t)0;
    /* initialize all to 1 (includes mu[1]) */
    for (idx_t i = 1; i <= n; ++i) mu[i] = 1;

    /* simple sieve of Eratosthenes to collect primes up to n */
    bool *is_comp = (bool *)calloc((size_t)n + 1, sizeof(bool));
    if (!is_comp) { free(mu); return (seq_t)0; }

    /* dynamic array for primes */
    size_t primes_cap = (size_t)(n / 10 + 10);
    uint32_t *primes = (uint32_t *)malloc(primes_cap * sizeof(uint32_t));
    if (!primes) { free(mu); free(is_comp); return (seq_t)0; }
    size_t primes_cnt = 0;

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_comp[(size_t)i]) {
            /* i is prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { free(mu); free(is_comp); free(primes); return (seq_t)0; }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
            for (idx_t j = i + i; j <= n; j += i) is_comp[(size_t)j] = true;
        }
    }

    /* compute Möbius values via the sieve */
    for (size_t pi = 0; pi < primes_cnt; ++pi) {
        uint32_t p = primes[pi];
        /* toggle sign for every multiple of p */
        for (idx_t j = p; j <= n; j += p) mu[(size_t)j] = -mu[(size_t)j];

        /* set to 0 every multiple of p^2 */
        idx_t p2 = (idx_t)p * (idx_t)p;
        if (p2 <= n) {
            for (idx_t j = p2; j <= n; j += p2) mu[(size_t)j] = 0;
        }
    }

    /* count squarefree k <= n that are relatively prime to n */
    seq_t count = 0;
    seq_t seq_n = (seq_t)n;
    for (idx_t k = 1; k <= n; ++k) {
        if (mu[(size_t)k] != 0) {          /* k is squarefree */
            seq_t seq_k = (seq_t)k;
            if (gcd(seq_k, seq_n) == 1) ++count;
        }
    }

    /* free temporary storage */
    free(mu);
    free(is_comp);
    free(primes);

    return count;
}
