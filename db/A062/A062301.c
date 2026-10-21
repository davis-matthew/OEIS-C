/*
 * A062301  Number of ways writing n-th prime as a sum of two primes.
 * https://oeis.org/A062301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if the n-th prime pₙ satisfies pₙ – 2 is prime,
 *            otherwise a(n) = 0. That is equivalent to counting the number of
 *            ways of writing pₙ as a sum of two primes (2 + (pₙ–2))
 * Range:     u8 n <= 3000, u16 n <= 3000, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Global mutable state for a modest sieve. */
static idx_t   sieve_limit   = 0;      /* current upper bound */
static bool*   sieve_array   = NULL;  /* prime flags, 0–sieve_limit */
static idx_t*  prime_list    = NULL;  /* list of primes up to sieve_limit */
static size_t  prime_count   = 0;      /* number of primes stored */

/* Simple sieve of Eratosthenes up to 'limit' (inclusive).  Reallocates
   the arrays if a larger limit is requested. */
static void
sieve_up_to(idx_t limit)
{
    if (limit <= sieve_limit)
        return;                       /* already sufficient */

    /* free previous data */
    free(sieve_array);
    free(prime_list);

    sieve_array = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!sieve_array) {
        sieve_limit = 0;
        return;
    }

    /* initialise all entries as prime */
    for (idx_t i = 0; i <= limit; ++i)
        sieve_array[i] = true;
    if (limit >= 0) { sieve_array[0] = false; }
    if (limit >= 1) { sieve_array[1] = false; }

    for (idx_t p = 2; p * p <= limit; ++p) {
        if (sieve_array[p]) {
            for (idx_t k = p * p; k <= limit; k += p)
                sieve_array[k] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve_array[i]) ++cnt;

    /* collect primes */
    prime_list = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!prime_list) {
        free(sieve_array);
        sieve_array   = NULL;
        prime_count   = 0;
        sieve_limit   = 0;
        return;
    }

    size_t idx = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (sieve_array[i]) prime_list[idx++] = i;

    prime_count = cnt;
    sieve_limit = limit;
}

/* Rough upper bound for the n-th prime:  pₙ < n(ln n + ln ln n) for n ≥ 6.
   For smaller n we use a hard‑coded generous bound.  The function always
   produces a value not less than the true n-th prime. */
static idx_t
estimate_upper_prime(idx_t n)
{
    if (n < 6)
        return 15;                /* 5th prime = 11 */
    double dn   = (double)n;
    double bound = dn * (log(dn) + log(log(dn)));
    return (idx_t)(bound + 3.0 * dn);   /* safety margin */
}

/* Ensure that the global sieve contains at least 'n' primes. */
static void
ensure_n_primes(idx_t n)
{
    if (prime_count >= n)
        return;                  /* already enough */

    idx_t limit = estimate_upper_prime(n);
    /* In the unlikely event the estimate was too low, double until we have
       enough primes.  This loop will terminate quickly. */
    while (prime_count < n) {
        sieve_up_to(limit);
        if (prime_count >= n)
            break;
        limit *= 2;
    }
}

/* Test whether x is marked prime in the current sieve.  The function
   returns false if x > sieve_limit (should not happen in this code). */
static bool
is_prime_in_sieve(idx_t x)
{
    if (x > sieve_limit)
        return false;
    return sieve_array[x];
}

/* Public entry point required by the task. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Make sure the sieve contains the n-th prime. */
    ensure_n_primes(n);
    if (prime_count < n)
        return 0;  /* should not occur if allocation succeeded */

    idx_t p = prime_list[n - 1];  /* 0‑based index into prime_list */

    if (p <= 2)
        return 0;  /* p = 2 has no representation with two primes */

    idx_t candidate = p - 2;
    return is_prime_in_sieve(candidate) ? (seq_t)1 : (seq_t)0;
}
