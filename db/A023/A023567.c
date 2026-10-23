/*
 * A023567  Convolution of A023531 and primes.
 * https://oeis.org/A023567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=0..n-1} A023531(n-i) * p(i+1)
 * Range:     u8 n <= 24, u16 n <= 500, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sieve_first_n_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A023531(n) = 1 iff n = k(k+3)/2 for some integer k >= 1, else 0. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sieve_first_n_primes.h"

static uint64_t *primes = NULL;
static size_t primes_cap = 0;

/* Ensure that the array `primes` contains at least the first n primes.*/
static void ensure_primes(idx_t n)
{
    if (primes_cap >= (size_t)n) return;

    free(primes);
    primes_cap = (size_t)n;
    primes = (uint64_t *)malloc(primes_cap * sizeof(uint64_t));
    if (!primes) return;            /* memory failure – the function will then
                                      likely crash on the first use */

    uint32_t *prime32 = NULL;
    size_t cnt = 0;
    sieve_first_n_primes(n, &prime32, &cnt);   /* cnt should equal n */

    for (size_t i = 0; i < cnt && i < primes_cap; ++i)
        primes[i] = (uint64_t)prime32[i];

    free(prime32);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    ensure_primes(n);

    seq_t sum = 0;

    /* iterate over all k with k(k+3)/2 <= n, i.e. the positions of A023531(n)=1 */
    for (idx_t k = 1; ; ++k)
    {
        idx_t j = k * (k + 3) / 2;   /* position where A023531 = 1 */
        if (j > n) break;

        /* prime index: n - j + 1 (1‑based), convert to 0‑based */
        idx_t p_idx = n - j;
        if (p_idx >= primes_cap) break;   /* safety for unexpected sizes */

        sum += (seq_t)primes[p_idx];
    }

    return sum;
}
