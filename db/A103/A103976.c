/*
 * A103976  Partial sums of A040976 (= primes-2).
 * https://oeis.org/A103976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n} (prime_k - 2) i.e. a(n) = (sum of first n
 *            primes) - 2*n Offset: 1 (a(1) = 0)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   estimate_nth_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/estimate_nth_prime.h"

/* --------------------------------------------------------------------- */
/* Estimate an upper bound for the n-th prime (for n >= 6)                 */

/* --------------------------------------------------------------------- */
/* Static state for memoization                                         */
/* --------------------------------------------------------------------- */
static bool psum_computed   = false;  /* whether psum has been generated */
static idx_t psum_max_n     = 0;      /* maximum n for which psum is valid */
static seq_t *psum          = NULL;   /* psum[i] = sum of first i primes, 1‑based */

/* --------------------------------------------------------------------- */
/* Build psum up to at least n primes                                      */
/* --------------------------------------------------------------------- */
static void build_psum(idx_t n)
{
    idx_t bound = estimate_nth_prime(n);
    while (1) {
        /* Allocate boolean array for the sieve */
        bool *is_prime = (bool *)malloc((size_t)(bound + 1) * sizeof(bool));
        if (!is_prime)
            abort(); /* memory allocation failed */

        /* Initialize */
        for (idx_t i = 0; i <= bound; ++i)
            is_prime[i] = true;
        is_prime[0] = false;
        is_prime[1] = false;

        /* Sieve of Eratosthenes */
        for (idx_t p = 2; p * p <= bound; ++p) {
            if (is_prime[p]) {
                for (idx_t q = p * p; q <= bound; q += p)
                    is_prime[q] = false;
            }
        }

        /* Count primes and accumulate sums */
        idx_t  count = 0;
        seq_t *new_psum = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        if (!new_psum)
            abort(); /* memory allocation failed */

        new_psum[0] = 0;
        for (idx_t i = 2; i <= bound && count < n; ++i) {
            if (is_prime[i]) {
                ++count;
                new_psum[count] = new_psum[count - 1] + (seq_t)i;
            }
        }

        free(is_prime);

        if (count >= n) {
            /* We have enough primes; replace old psum */
            if (psum)
                free(psum);
            psum = new_psum;
            psum_max_n = count;
            psum_computed = true;
            break;
        }

        /* Not enough primes – try a larger bound */
        free(new_psum);
        bound *= 2;
    }
}

/* --------------------------------------------------------------------- */
/* Public function a(n)                                                */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* off‑offset values are irrelevant */

    if (!psum_computed || n > psum_max_n)
        build_psum(n);

    seq_t sum = psum[n];                  /* sum of first n primes   */
    seq_t res = sum - (seq_t)(2 * n);     /* minus 2*n               */
    return res;
}
