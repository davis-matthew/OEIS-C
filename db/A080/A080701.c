/*
 * A080701  Product of upper bound twin-prime-indexed primes and their upper
 *          bound twin prime.
 * https://oeis.org/A080701
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = prime(Q) * Q , where Q is the upper member of the n‑th
 *            twin prime pair (twin(n) = (P,Q))
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for C/C++17 compatibility */
#include <stdlib.h>   /* malloc, calloc, free */
#include <stdint.h>   /* uint32_t, uint8_t */
#include <stdbool.h>  /* bool */
#include <stddef.h>   /* size_t */
#include <math.h>     /* log */

static uint32_t *primes   = NULL;      /* list of prime values */
static size_t    prime_cnt = 0;        /* number of primes stored */
static uint32_t *twin_Qs  = NULL;      /* upper members of twin prime pairs */
static size_t    twin_cnt  = 0;        /* number of twin pairs stored */
static uint32_t  sieve_limit = 0;      /* current upper bound of the sieve */

/* ------------------------------------------------------------------ */
/* Helper: Build a sieve up to `limit`, filling `primes` and `twin_Qs`. */
/* ------------------------------------------------------------------ */
static void build_tables(uint32_t limit)
{
    /* Free any previously allocated memory */
    if (primes)   free(primes);
    if (twin_Qs)  free(twin_Qs);

    /* Create a marker array for composite numbers */
    uint8_t *is_comp = (uint8_t*)calloc((size_t)limit + 1, 1);
    if (!is_comp) exit(1);   /* allocation failed */

    /* Classic sieve of Eratosthenes */
    for (uint32_t i = 2; (uint64_t)i * i <= limit; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= limit; j += i)
                is_comp[j] = 1;
        }
    }

    /* Estimate an upper bound for the number of primes <= limit */
    size_t est = (size_t)(limit / (log((double)limit) + 1.0)) + 10;
    primes = (uint32_t*)malloc(est * sizeof(uint32_t));
    if (!primes) exit(1);
    prime_cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        if (!is_comp[i]) {
            if (prime_cnt == est) {
                est = est + est / 2;
                primes = (uint32_t*)realloc(primes, est * sizeof(uint32_t));
                if (!primes) exit(1);
            }
            primes[prime_cnt++] = i;
        }
    }

    /* Collect the upper members of twin prime pairs */
    size_t twin_est = prime_cnt / 2 + 10;
    twin_Qs = (uint32_t*)malloc(twin_est * sizeof(uint32_t));
    if (!twin_Qs) exit(1);
    twin_cnt = 0;
    for (size_t i = 1; i < prime_cnt; ++i) {
        if (primes[i] - primes[i-1] == 2) {
            if (twin_cnt == twin_est) {
                twin_est = twin_est + twin_est / 2;
                twin_Qs = (uint32_t*)realloc(twin_Qs, twin_est * sizeof(uint32_t));
                if (!twin_Qs) exit(1);
            }
            twin_Qs[twin_cnt++] = primes[i]; /* upper element of the pair */
        }
    }

    free(is_comp);
    sieve_limit = limit;
}

/* ------------------------------------------------------------------ */
/* Helper: Ensure the tables contain at least `n` twin pairs.         */
/* The sieve upper bound is increased geometrically until enough     */
/* data is available.                                                   */
/* ------------------------------------------------------------------ */
static void ensure_tables(idx_t n)
{
    /* If we already have enough data, just return */
    if (twin_cnt >= (size_t)n)
        return;

    /* Estimate a reasonable sieve limit for the desired nth twin.
       For n >= 3 the nth twin prime is roughly n*(log n)^2. */
    double dn = (double)n;
    uint32_t estimate_limit;
    if (n < 3) {
        estimate_limit = 1000U;            /* small hard‑coded value */
    } else {
        double val = dn * (log(dn) * log(dn));
        estimate_limit = (uint32_t)(val * 1.5) + 100;  /* cushion factor */
    }

    /* Ensure the limit is at least the current one */
    if (estimate_limit < sieve_limit)
        estimate_limit = sieve_limit;

    /* Build tables with the estimated limit.  If the resulting
       table still does not contain `n` twin pairs, double the limit
       and rebuild until we have enough. */
    while (1) {
        build_tables(estimate_limit);
        if (twin_cnt >= (size_t)n)
            break;
        estimate_limit = estimate_limit * 2;
    }
}

/* ------------------------------------------------------------------ */
/* Public function: compute a(n).                                      */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* undefined for n < 1 */

    /* Build enough data to satisfy the request */
    ensure_tables(n);

    /* Retrieve the upper member of the nth twin pair */
    uint32_t Q = twin_Qs[(size_t)n - 1];
    if (Q == 0)
        return 0;          /* safety check */

    /* Ensure that we have at least Q primes (so that primes[Q-1] exists) */
    while (prime_cnt <= (size_t)Q) {
        /* Double the sieve limit and rebuild */
        uint32_t new_limit = sieve_limit * 2;
        if (new_limit <= sieve_limit)  /* guard against overflow */
            new_limit = (uint32_t)(sieve_limit + 100000);
        build_tables(new_limit);
    }

    /* Now primes[Q-1] is the Q‑th prime number */
    seq_t prime_of_Q = (seq_t)primes[(size_t)Q - 1];
    seq_t result = prime_of_Q * (seq_t)Q;
    return result;
}
