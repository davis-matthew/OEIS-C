/*
 * A098235  Number of ways to write n as a sum of two ordered positive
 *          squarefree numbers.
 * https://oeis.org/A098235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n-1} ( μ(k)·μ(n-k) )² where μ is the Möbius
 *            function
 * Range:     u8 n <= 467, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes μ up to n with a linear sieve
   and then counts the pairs of square‑free numbers that sum to n. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n <= 1 there are no ordered pairs. */
    if (n <= 1) {
        return (seq_t)0;
    }

    /* Allocate arrays for the Möbius values and a composite flag. */
    size_t len = (size_t)n + 1;
    int8_t *mu        = (int8_t*)calloc(len, sizeof(int8_t));
    uint8_t *is_compr = (uint8_t*)calloc(len, sizeof(uint8_t));
    if (!mu || !is_compr) {
        /* Allocation failure – return 0 (unlikely for the tested limits). */
        free(mu);
        free(is_compr);
        return (seq_t)0;
    }

    /* Linear sieve to fill μ[1..n] and identify composites. */
    mu[1] = 1;
    size_t primes_cap = 128;
    uint32_t *primes = (uint32_t*)malloc(primes_cap * sizeof(uint32_t));
    size_t primes_cnt = 0;
    if (!primes) {
        free(mu);
        free(is_compr);
        return (seq_t)0;
    }

    for (idx_t i = 2; i <= n; ++i) {
        if (!is_compr[(size_t)i]) {
            /* i is prime */
            if (primes_cnt == primes_cap) {
                primes_cap *= 2;
                uint32_t *tmp = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
                if (!tmp) { /* allocation failure – clean up */
                    free(primes);
                    free(mu);
                    free(is_compr);
                    return (seq_t)0;
                }
                primes = tmp;
            }
            primes[primes_cnt++] = (uint32_t)i;
            mu[(size_t)i] = -1;
        }
        for (size_t pj = 0; pj < primes_cnt; ++pj) {
            uint32_t p = primes[pj];
            idx_t ip = i * p;
            if (ip > n) break;
            is_compr[(size_t)ip] = 1;
            if (i % p == 0) {
                mu[(size_t)ip] = 0;
                break;
            } else {
                mu[(size_t)ip] = -mu[(size_t)i];
            }
        }
    }

    /* Count the pairs (k, n-k) where both numbers are square‑free. */
    seq_t res = (seq_t)0;
    for (idx_t k = 1; k < n; ++k) {
        int8_t mk   = mu[(size_t)k];
        int8_t mnk  = mu[(size_t)(n - k)];
        if (mk && mnk) {          /* both μ values are ±1 → product ±1 → square 1 */
            res += (seq_t)1;
        }
    }

    /* Free temporary storage. */
    free(primes);
    free(mu);
    free(is_compr);

    return res;
}
