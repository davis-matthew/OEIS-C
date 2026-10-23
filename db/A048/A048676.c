/*
 * A048676  Binary encoding of factorizations, alternative 2, a(n) = bef2(n);.
 * https://oeis.org/A048676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/4 * Σ 2^(i+e_i) over prime powers p_i^e_i dividing n,
 *            where p_i is the i‑th prime (p_1 = 2)
 * Range:     u8 n <= 16, u16 n <= 46, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/pow2.h"
#include "../utils/factor_u64.h"
#include "../utils/build_primes_upto.h"

/* Utility: find the 1‑based index of prime `p` in the globally cached
   prime table.  It returns 0 if p is not found (which should never
   happen for the factorisation of an integer). */
static size_t prime_index(uint64_t p, const uint64_t *tbl, size_t count)
{
    size_t lo = 0, hi = count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (tbl[mid] == p) return mid + 1;      /* 1‑based */
        if (tbl[mid] < p) lo = mid + 1;
        else hi = mid;
    }
    return 0;  /* not found */
}

static bool primes_initialized = false;
static uint64_t *cached_primes = NULL;
static size_t cached_prime_count = 0;

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;

    /* Initialise the prime cache once.  We choose a limit that
       comfortably exceeds the largest prime factor we will encounter
       for n up to 10000 (the maximum tested by the framework). */
    if (!primes_initialized) {
        size_t cnt = 0;
        uint64_t *pr = NULL;
        build_primes_upto(200000, &pr, &cnt);  /* supplies primes ≤ 200000 */
        cached_primes = pr;
        cached_prime_count = cnt;
        primes_initialized = true;
    }

    /* Factor n.  We can use factor_u64 which writes into the buffers. */
    uint64_t primes[64], exponents[64];
    size_t pf_count = 0;
    factor_u64((uint64_t)n, primes, exponents, &pf_count);

    seq_t sum = 0;
    for (size_t j = 0; j < pf_count; ++j) {
        uint64_t prime = primes[j];
        uint64_t exp = exponents[j];
        size_t  idx   = prime_index(prime, cached_primes, cached_prime_count);
        /* idx is 1‑based, exp is the exponent.  Compute 2^(idx+exp). */
        seq_t term = pow2((idx_t)(idx + exp));
        sum += term;
    }

    /* Divide by 4.  The sum is always a multiple of 4. */
    sum /= 4;

    return sum;
}
