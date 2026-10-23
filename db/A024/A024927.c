/*
 * A024927  a(n) = Sum_{k=1..n} k*floor( prime(k)/k ).
 * https://oeis.org/A024927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} k*floor( prime(k)/k )
 * Range:     u8 n <= 14, u16 n <= 165, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static cache data */
static idx_t precomp_n = 0;          /* largest n for which cache is built */
static uint32_t *prime_cache = NULL; /* first precomp_n primes */
static seq_t   *sum_cache   = NULL; /* cumulative sums a(1)..a(precomp_n) */
static size_t  cache_cap   = 0;      /* allocated capacity of prime and sum arrays */

/* Ensure that a(n) is available for all n <= req_n */
static void ensure_precomp(idx_t req_n)
{
    if (precomp_n >= req_n)
        return;                         /* already computed */

    /* Estimate an upper bound for the req_n-th prime. */
    idx_t limit;
    if (req_n < 6)
        limit = 15; /* small hard‑coded bound */
    else {
        double dn = (double)req_n;
        /* upper bound: n(log n + log log n) + 3n, plus safety margin */
        limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 3.0 * dn) + 10;
    }

    /* Allocate/resize arrays to hold all needed primes and sums. */
    if (cache_cap < (size_t)req_n) {
        free(prime_cache);
        free(sum_cache);
        prime_cache = (uint32_t*)malloc(req_n * sizeof(uint32_t));
        sum_cache   = (seq_t  *)malloc(req_n * sizeof(seq_t));
        cache_cap   = req_n;
    }

    /* Repeatedly try with larger limits until we obtain enough primes. */
    size_t primes_needed = (size_t)req_n;
    size_t primes_found = 0;
    while (true) {
        /* Allocate flag array for sieve. */
        bool *is_comp = (bool*)calloc((size_t)limit + 1, sizeof(bool));
        if (!is_comp) exit(1);

        primes_found = 0;
        for (idx_t p = 2; p <= limit && primes_found < primes_needed; ++p) {
            if (!is_comp[(size_t)p]) {
                prime_cache[primes_found++] = (uint32_t)p;
                if ((double)p * (double)p <= (double)limit) {
                    for (idx_t m = p * p; m <= limit; m += p)
                        is_comp[(size_t)m] = true;
                }
            }
        }
        free(is_comp);

        if (primes_found >= primes_needed)
            break;                      /* sufficient primes */
        limit *= 2;                      /* increase bound and retry */
    }

    /* Build cumulative sums a(1)..a(req_n). */
    for (size_t k = 0; k < primes_needed; ++k) {
        idx_t kk = (idx_t)(k + 1);                 /* current index k+1 */
        seq_t floor_val = (seq_t)(prime_cache[k] / kk);
        seq_t term = (seq_t)kk * floor_val;
        if (k == 0)
            sum_cache[0] = term;
        else
            sum_cache[k] = sum_cache[k - 1] + term;
    }

    precomp_n = req_n;
}

/* Public function as required by OEIS definition. */
seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1; n=0 is outside the range */
        return 0;

    ensure_precomp(n);
    return sum_cache[n - 1];
}
