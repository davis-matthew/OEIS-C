/*
 * A061215  a(n)= product of all odd composite numbers between n-th prime and
 *          (n+1)-st prime.
 * https://oeis.org/A061215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all odd composite numbers between * the n-th
 *            prime (exclusive) and the (n+1)-st prime (exclusive)
 * Range:     u8 n <= 8, u16 n <= 23, u32 n <= 29, u64 n <= 98, u128 n <= 216,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Globals for prime storage */
static uint8_t *g_sieve    = NULL;   /* 1 = prime, 0 = composite */
static idx_t   g_limit     = 0;      /* current sieve limit (inclusive) */
static idx_t  *g_primes    = NULL;   /* array of primes, 0‑based */
static size_t  g_prime_cnt = 0;      /* number of primes stored */

/* ---------------------------------------------------------------------- */
/*  Ensure that at least 'need' primes are available (need ≥ 1).            */
/* ---------------------------------------------------------------------- */
static void ensure_primes(idx_t need)
{
    /* If we already have enough primes, do nothing. */
    if (g_prime_cnt >= (size_t)need)
        return;

    /* Start or resize the sieve to an adequate limit. */
    /* A good heuristic: p(n) < n (log n + log log n) for n ≥ 6. */
    idx_t limit = g_limit;
    if (limit == 0) {
        /* Enough for n up to about 17000 (the 17000‑th prime is ≈ 196933). */
        limit = 200000;
    }

    /* Re‑sieve until we get enough primes. */
    for (;;) {
        /* Allocate sieve memory. */
        uint8_t *sieve = (uint8_t *)malloc((size_t)(limit + 1));
        if (!sieve)
            break;  /* out of memory – give up */

        /* Simple Eratosthenes sieve. */
        memset(sieve, 1, (size_t)(limit + 1));
        sieve[0] = sieve[1] = 0;
        for (idx_t i = 2; i * i <= limit; ++i) {
            if (sieve[i]) {
                for (idx_t j = i * i; j <= limit; j += i)
                    sieve[j] = 0;
            }
        }

        /* Count primes up to 'limit'. */
        size_t max_primes = limit / 2 + 10;  /* generous upper bound */
        idx_t *primes = (idx_t *)malloc(max_primes * sizeof(idx_t));
        if (!primes) {
            free(sieve);
            break;
        }

        size_t pc = 0;
        for (idx_t i = 2; i <= limit; ++i) {
            if (sieve[i]) {
                if (pc == max_primes) {
                    /* Grow the array if necessary. */
                    max_primes *= 2;
                    idx_t *tmp = (idx_t *)realloc(primes,
                                                  max_primes * sizeof(idx_t));
                    if (!tmp) {
                        free(sieve);
                        free(primes);
                        return;
                    }
                    primes = tmp;
                }
                primes[pc++] = i;
            }
        }

        /* Have we got enough primes? */
        if (pc >= (size_t)need) {
            /* Replace old data. */
            free(g_sieve);
            free(g_primes);
            g_sieve   = sieve;
            g_primes  = primes;
            g_limit   = limit;
            g_prime_cnt = pc;
            return;
        }

        /* Not enough primes – clean up and try again with a larger limit. */
        free(sieve);
        free(primes);
        limit *= 2;  /* double the search range */
    }
}

/* ---------------------------------------------------------------------- */
/*  Sequence function                                                     */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)1;   /* sequence defined only for n ≥ 1 */

    /* Obtain primes up to (n+1)-st prime. */
    ensure_primes(n + 1);

    idx_t left  = g_primes[n - 1];   /* n‑th prime (0‑based index) */
    idx_t right = g_primes[n];       /* (n+1)-st prime */

    seq_t prod = (seq_t)1;
    for (idx_t x = left + 1; x < right; ++x) {
        if ((x & 1U) && !g_sieve[x])   /* odd composite */
            prod *= (seq_t)x;
    }
    return prod;
}
