/*
 * A107712  a(n) = Product_{k=1..n} prime(k+n).
 * https://oeis.org/A107712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} prime(k+n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   (a(0) = 1 by convention)                                      */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

/* ------------------------------------------------------------------ */
/*   Prime generation helpers                                         */
/* ------------------------------------------------------------------ */
static uint32_t *prime_list = NULL;   /* 0‑based: prime_list[i] = prime(i+1)           */
static size_t    prime_count = 0;     /* number of primes stored                      */
static bool *is_composite = NULL;     /* sieve array, size = sieve_limit+1            */
static size_t   sieve_limit   = 0;     /* last number sieved                           */

/* Estimate an upper bound for the nth prime (n ≥ 1).  The bound is safe
   for n ≥ 6.  For n < 6 the value 15 is enough.                         */
static uint64_t estimate_prime_upper_bound(size_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double bound = dn * (log(dn) + log(log(dn)));
    return (uint64_t)(bound) + 3;      /* small safety margin */
}

/* Ensure that prime_list contains at least the first 2*n primes.    */
static void ensure_primes(idx_t n)
{
    size_t needed = 2ULL * (size_t)n;
    if (prime_count >= needed) return;   /* already enough */

    uint64_t new_limit;
    if (needed < 6)
        new_limit = 15;
    else
        new_limit = estimate_prime_upper_bound(needed);

    if (new_limit <= sieve_limit) {
        /* we already sieved far enough, just rebuild the list */
        new_limit = sieve_limit;
    }

    /* Reallocate/complement the sieve array */
    is_composite = (bool*)realloc(is_composite, (new_limit + 1) * sizeof(bool));
    if (!is_composite) return;   /* allocation failed – safe fallback */
    if (new_limit > sieve_limit) {
        /* zero the new part if we extended */
        memset(is_composite + sieve_limit, 0,
               (new_limit - sieve_limit + 1) * sizeof(bool));
    }
    sieve_limit = (size_t)new_limit;

    /* Run Eratosthenes sieve from scratch */
    memset(is_composite, 0, (sieve_limit + 1) * sizeof(bool));
    for (size_t p = 2; p * p <= sieve_limit; ++p) {
        if (!is_composite[p]) {
            for (size_t m = p * p; m <= sieve_limit; m += p)
                is_composite[m] = true;
        }
    }

    /* Count primes and rebuild the list */
    size_t new_count = 0;
    for (size_t i = 2; i <= sieve_limit; ++i)
        if (!is_composite[i]) new_count++;

    uint32_t *new_list = (uint32_t*)realloc(prime_list, new_count * sizeof(uint32_t));
    if (!new_list) return;  /* allocation failed – safe fallback */

    size_t idx = 0;
    for (size_t i = 2; i <= sieve_limit; ++i)
        if (!is_composite[i]) new_list[idx++] = (uint32_t)i;

    prime_list = new_list;
    prime_count = new_count;
}

/* ------------------------------------------------------------------ */
/*   Sequence implementation                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;               /* empty product */

    ensure_primes(n);
    if (prime_count < 2ULL * (size_t)n) return (seq_t)0;  /* shouldn't happen */

    seq_t prod = (seq_t)1;
    /* product of primes with indices (n+1)..(2n) → indices n..2n-1 in 0‑based array */
    for (idx_t i = n; i < (idx_t)(2ULL * n); ++i)
        prod *= (seq_t)prime_list[(size_t)i];

    return prod;
}
