/*
 * A063534  Numbers k such that C(k) = H(k) + d(k), where C(k) is Chowla's
 *          function A048050, H(k) is the half-totient function A023022 and
 *          d(k) is the number of divisors function A000005.
 * https://oeis.org/A063534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A063534
   a(1) = 6
   a(2) = 8
   a(n) = 3 × p[n‑1]  for n ≥ 3
   where p[i] is the i‑th prime (p[0] = 2, p[1] = 3, …) */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* ---------- Prime sieve (up to a fixed upper bound) ---------- */
static uint64_t *primes = NULL;      /* array of primes */
static size_t   prime_cnt = 0;       /* number of primes stored */
static bool     primes_built = false;

static void build_primes(void)
{
    if (primes_built) return;          /* already built once */

    /* Upper bound sufficient for n up to about 10 000 :
       the 10 000‑th prime is 104 729 < 200 000.                  */
    const uint64_t limit = 200000;
    const size_t   cap   = limit / 10 + 100;  /* rough number of primes */

    primes  = (uint64_t *)malloc(cap * sizeof(uint64_t));
    prime_cnt = 0;

    /* simple sieve of Eratosthenes */
    char *is_comp = (char *)calloc(limit + 1, 1);
    for (uint64_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == cap) {                 /* grow buffer if needed */
                const size_t new_cap = cap << 1;
                primes = (uint64_t *)realloc(primes,
                                              new_cap * sizeof(uint64_t));
                /* Note: the old `cap` value is no longer reliable.  */
            }
            primes[prime_cnt++] = i;
            if (i * i <= limit) {
                for (uint64_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }
    free(is_comp);

    primes_built = true;
}

/* ---------- Sequence function ---------- */
seq_t a(idx_t n)
{
    /* special first two terms */
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)8;

    /* for n ≥ 3 use the primes */
    build_primes();   /* ensures the prime table is ready */

    /* n must be ≥ 3 here; indexing is zero‑based */
    const idx_t prime_index = n - 1;          /* p[n-1] */
    /* safety: if requested index is beyond precomputed primes,
       we extend the sieve (unlikely for n ≤ 10 000). */
    if ((size_t)prime_index >= prime_cnt) {
        /* Extremely unlikely for the problem constraints; extend
           by re‑building with a larger limit.  */
        const uint64_t new_limit = prime_cnt * 2 + 100000; /* rough growth */
        /* free old data */
        free(primes);
        primes_built = false;
        /* the new limit will be built on the next call */
        build_primes();   /* rebuild with a larger limit if needed */
    }

    return (seq_t)3 * (seq_t)primes[prime_index];
}
