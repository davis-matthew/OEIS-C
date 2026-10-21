/*
 * A097240  a(n) = (n+1)*prime(n) + n*prime(n+1).
 * https://oeis.org/A097240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 79, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A097240
   a(n) = (n+1)*prime(n) + n*prime(n+1)
   (offset 1: a(1) = 7, a(2) = 19, a(3) = 41, …)
   Computes the nth term using an on‑demand sieve to generate primes.
   Uses the standard upper bound p_n <= n (log n + log log n) for n >= 6,
   supplemented with a small margin.  Intermediate arithmetic is done
   in unsigned __int128 to avoid overflow in any supported build.  */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the n-th and (n+1)-th primes (1‑based).  */
    static size_t primes_needed = 0;    /* number of primes currently stored (>=2)   */
    static size_t primes_alloc   = 0;    /* capacity of the primes array              */
    static uint64_t *primes      = NULL; /* array of primes, primes[0] = 2             */

    if (n < 1u) return 0; /* behavior below offset is undefined */

    size_t need = (size_t)n + 1;   /* we need n+1 primes (index 1..n+1) */
    if (need > primes_needed) {
        /* Estimate an upper bound for the (need)th prime using Rosser's theorem */
        size_t limit;
        if (need < 6) {
            limit = 15;                  /* primes up to 13 are enough */
        } else {
            double dn  = (double)need;
            double lb  = log(dn);
            double llog = log(lb);
            double upper = dn * (lb + llog) + 3.0; /* add a small margin */
            limit = (size_t)upper + 10; /* extra safety */
        }

        /* Allocate boolean array for sieve */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime) return 0; /* allocation failure – unlikely for limits used */

        for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;

        for (size_t p = 2; p * p <= limit; ++p) {
            if (is_prime[p]) {
                for (size_t mult = p * p; mult <= limit; mult += p)
                    is_prime[mult] = false;
            }
        }

        /* Count primes and reallocate array if necessary */
        size_t cnt = 0;
        for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;

        if (cnt < need) { /* Rare case – extend limit and retry */
            free(is_prime);
            need = cnt; /* We'll recompute after extending */
            /* Simple strategy: double the limit until we have enough primes */
            while (cnt < need) {
                limit <<= 1; /* double */
                is_prime = (bool *)realloc(is_prime, (limit + 1) * sizeof(bool));
                for (size_t i = 2; i <= limit; ++i) is_prime[i] = true;
                for (size_t p = 2; p * p <= limit; ++p) {
                    if (is_prime[p]) {
                        for (size_t mult = p * p; mult <= limit; mult += p)
                            is_prime[mult] = false;
                    }
                }
                cnt = 0;
                for (size_t i = 2; i <= limit; ++i) if (is_prime[i]) ++cnt;
            }
        }

        /* Build primes array */
        uint64_t *new_primes = (uint64_t *)realloc(primes, need * sizeof(uint64_t));
        if (!new_primes) { free(is_prime); return 0; }
        primes = new_primes;

        size_t idx = 0;
        for (size_t i = 2; i <= limit && idx < need; ++i) {
            if (is_prime[i]) primes[idx++] = i;
        }

        primes_needed = need;
        free(is_prime);
    }

    /* Retrieve required primes */
    uint64_t p_n   = primes[(size_t)n - 1]; /* prime(n) */
    uint64_t p_np1 = primes[(size_t)n];     /* prime(n+1) */

    /* Compute a(n) using 128‑bit arithmetic to avoid overflow */
    unsigned __int128 term = (unsigned __int128)(n + 1) * p_n
                           + (unsigned __int128)n * p_np1;

    return (seq_t)term;
}
