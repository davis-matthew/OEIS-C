/*
 * A127337  Numbers that are the sum of 10 consecutive primes.
 * https://oeis.org/A127337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=n}^{n+9} prime(k) – the sum of 10 consecutive primes
 * Range:     u8 n <= 4, u16 n <= 842, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Static cache for the primes. */
static uint64_t *primes = NULL;   /* array of primes (0‑based) */
static size_t primes_count = 0;   /* number of primes stored */
static size_t sieve_limit = 0;    /* maximum value sieved so far */

/* Estimate an upper bound on the value of the n‑th prime.  Uses the known
   bound p_n < n (log n + log log n) for n ≥ 6.  For n < 6 we use 15. */
static size_t estimate_limit(idx_t n)
{
    if (n < 6) return 15;
    double dn = (double)n;
    double lim = dn * (log(dn) + log(log(dn))) + 3.0; /* small safety margin */
    return (size_t)lim;
}

/* Sieve up to `limit` inclusive and store all primes in the static array. */
static void sieve_primes_upto(size_t limit)
{
    /* Allocate sieve array.  Each entry holds 0/1. */
    char *is_prime = (char *)calloc(limit + 1, sizeof(char));
    if (!is_prime) return;           /* out of memory – abort silently */

    for (size_t i = 2; i <= limit; ++i) is_prime[i] = 1;
    for (size_t i = 2; i * i <= limit; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= limit; j += i)
                is_prime[j] = 0;
        }
    }

    /* Count primes to size the array. */
    size_t cnt = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) ++cnt;

    /* Reallocate array if needed. */
    if (cnt > primes_count) {
        uint64_t *new_arr = (uint64_t *)realloc(primes, cnt * sizeof(uint64_t));
        if (!new_arr) { free(is_prime); return; }
        primes = new_arr;
        primes_count = 0;                 /* will repopulate below           */
    }

    /* Fill array. */
    for (size_t i = 2; i <= limit; ++i)
        if (is_prime[i]) primes[primes_count++] = (uint64_t)i;

    free(is_prime);
}

/* Ensure that primes[0..needed-1] are available (i.e., at least `needed`
   primes have been computed). */
static void ensure_primes_needed(idx_t needed)
{
    if ((size_t)needed <= primes_count) return;   /* we already have enough */

    /* Determine value limit for the nth prime. */
    size_t limit = estimate_limit(needed);
    if (limit <= sieve_limit) {
        /* The previous sieve already covered the needed values, but our
           array was truncated.  Re‑populate from the existing sieve. */
        sieve_primes_upto(limit);
        return;
    }
    /* New sieve required. */
    sieve_limit = limit;
    sieve_primes_upto(limit);
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Need primes with indices n ... n+9 (1‑based). */
    ensure_primes_needed(n + 9);

    /* Compute sum of 10 consecutive primes. */
    seq_t sum = 0;
    for (idx_t k = n; k <= n + 9; ++k) {
        sum += (seq_t)primes[k - 1];   /* array is 0‑based */
    }
    return sum;
}
