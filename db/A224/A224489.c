/*
 * A224489  Smallest k such that k*2*p(n)^2-1 is prime.
 * https://oeis.org/A224489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 2225, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Smallest k such that k*2*p(n)^2-1 is prime.
 * p(n) = n-th prime: 2,3,5,7,11,...
 * a(n) = min{k ≥ 1 | (k*2*(p(n))^2 - 1) is prime}
 *
 * Uses a simple incremental sieve to obtain the n-th prime for any n ≥ 1.
 * On first call (or when a larger n is requested) the sieve is redone
 * at an over‑estimate of the n-th prime, n*20+10, which is safely
 * above the actual nth prime for n ≤ 10 000.
 *
 * Primality testing is performed by isPrimeGeneric from the utils
 * library which accepts seq_t arguments (arbitrary‑precision if
 * necessary).
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>          /* for sqrt, but only on double values */
#include "../utils/isPrimeGeneric.h"

/* ------------------------------------------------------------------
 *  Prime storage and sieve helpers
 * ------------------------------------------------------------------ */
static uint64_t *prime_array = NULL;   /* array of primes (uint64_t) */
static size_t prime_count  = 0;        /* number of primes stored   */
static uint64_t sieve_limit = 0;       /* current upper bound used  */

static void ensure_primes_for(idx_t n)
{
    /* return if we already have at least n primes */
    if (prime_count >= (size_t)n)
        return;

    /* compute a conservative upper bound for the n-th prime
     *  n*20+10 is safely above p(n) for n ≤ 10000
     *  (p(10000)=104729) */
    uint64_t new_limit = (uint64_t)n * 20ull + 10ull;
    if (new_limit <= sieve_limit)
        return;    /* old sieve covers this already */

    /* free previous data (if any) */
    free(prime_array);
    prime_array = NULL;
    free((void *)NULL);  /* nothing to free for flag array, we allocate anew */

    /* allocate sieve flag array of size new_limit+1 */
    uint8_t *flags = (uint8_t *)malloc((size_t)(new_limit + 1));
    if (!flags)            /* allocation failure → abort */
        abort();

    /* initialise to true (1) */
    for (size_t i = 0; i <= new_limit; ++i)
        flags[i] = 1;
    flags[0] = flags[1] = 0;

    /* simple Eratosthenes */
    uint64_t sq = (uint64_t)floor(sqrt((double)new_limit));
    for (uint64_t i = 2; i <= sq; ++i) {
        if (flags[i]) {
            for (uint64_t j = i * i; j <= new_limit; j += i)
                flags[j] = 0;
        }
    }

    /* count primes and allocate array */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= new_limit; ++i)
        if (flags[i]) ++cnt;

    uint64_t *pr = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!pr)
        abort();

    size_t idx = 0;
    for (uint64_t i = 2; i <= new_limit; ++i)
        if (flags[i]) pr[idx++] = i;

    /* free flags */
    free(flags);

    prime_array  = pr;
    prime_count  = cnt;
    sieve_limit  = new_limit;
}

/* ------------------------------------------------------------------
 *  Main function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)  /* undefined for n=0, but just return 0 */
        return (seq_t)0;

    /* ensure we have the nth prime */
    ensure_primes_for(n);

    /* index into zero‑based array */
    uint64_t p = prime_array[(size_t)(n - 1)];
    seq_t   p_ts = (seq_t)p;

    /* square of the prime as seq_t */
    seq_t pp = p_ts * p_ts;              /* p^2 */

    /* search for the smallest k */
    for (uint64_t k = 1;; ++k) {
        seq_t candidate = ((seq_t)k * 2 * pp) - 1;
        if (isPrimeGeneric(candidate))
            return (seq_t)k;
    }
}
