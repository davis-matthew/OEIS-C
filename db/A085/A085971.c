/*
 * A085971  Union of primes and numbers that are not prime powers (A000040,
 *          A024619).
 * https://oeis.org/A085971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A085971: numbers that are either prime or not a prime power.
   Computation: Sieve primes up to a limit, mark prime powers with exponent ≥2,
   then enumerate integers ≥2 and keep those not marked as prime powers.
   Returns a(n) for n≥1. */
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *seq = NULL;      /* array of terms already generated */
    static idx_t seq_len = 0;      /* number of terms stored */
    static idx_t highest_n = 0;    /* highest n computed so far */

    /* If we already have the term, return it immediately */
    if (n <= seq_len)
        return seq[n - 1];

    /* Estimate an upper bound for a(n) using a(n) ≈ n + 2√n + 100 */
    size_t need = (size_t)n;
    size_t limit = (size_t)(need + 2.0 * sqrt((double)need) + 100.0);

    /* Recompute the sequence until it contains at least 'need' terms */
    while (1)
    {
        /* Allocate auxiliary arrays */
        unsigned char *is_composite = (unsigned char *)calloc(limit + 1, 1);
        if (!is_composite)
            return 0; /* out‑of‑memory guard – never used in tests */

        unsigned char *is_prime_power = (unsigned char *)calloc(limit + 1, 1);
        if (!is_prime_power) {
            free(is_composite);
            return 0;
        }

        /* Prime sieve and list of primes up to 'limit' */
        int *primes = (int *)malloc((limit + 1) * sizeof(int));
        int primes_cnt = 0;
        for (size_t i = 2; i <= limit; ++i) {
            if (!is_composite[i]) {
                primes[primes_cnt++] = (int)i;
                if (i * i <= limit) {
                    for (size_t j = i * i; j <= limit; j += i)
                        is_composite[j] = 1;
                }
            }
        }

        /* Mark all prime powers p^k with k ≥ 2 */
        for (int k = 0; k < primes_cnt; ++k) {
            size_t p = (size_t)primes[k];
            size_t val = p * p;
            while (val <= limit) {
                is_prime_power[val] = 1;
                if (val > limit / p) break; /* avoid overflow */
                val *= p;
            }
        }

        /* Generate or extend the sequence array */
        size_t new_cap = need * 2 + 10;  /* sufficient capacity */
        seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!new_seq) {
            free(is_composite);
            free(is_prime_power);
            free(primes);
            return 0;
        }
        seq = new_seq;

        idx_t idx = 0;
        for (size_t i = 2; i <= limit && idx < need; ++i) {
            if (!is_prime_power[i]) {
                seq[idx++] = (seq_t)i;   /* cast to seq_t type */
            }
        }

        /* Release temporary arrays */
        free(is_composite);
        free(is_prime_power);
        free(primes);

        /* If we have enough terms, store the count and return the requested value */
        if ((idx_t)idx >= need) {
            seq_len = idx;
            return seq[n - 1];
        }

        /* Not enough terms – increase limit and try again */
        limit = limit * 2 + 100;  /* grow bound */
    }
}
