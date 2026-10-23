/*
 * A104897  Difference between (n+prime(n)) and next prime.
 * https://oeis.org/A104897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nextPrime(n + prime(n)) - (n + prime(n))
 * Range:     u8 n <= 46, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   estimate_prime_upper_bound
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/estimate_prime_upper_bound.h"

/* Global storage for the sieve and prime list */
static bool   *is_prime   = NULL;   /* sieve array, indexed by natural number */
static idx_t  *primes_arr = NULL;   /* list of primes, 0‑based index */
static idx_t   num_primes = 0;      /* number of primes stored */
static idx_t   sieve_limit = 0;     /* current maximum number sieved */

/* Re‑build the sieve up to `limit`.  Frees any existing data. */
static void rebuild_sieve(idx_t limit)
{
    /* Allocate sieve array */
    bool *new_is_prime = (bool*)malloc((limit + 1) * sizeof(bool));
    if (!new_is_prime) return;

    for (idx_t i = 0; i <= limit; ++i)
        new_is_prime[i] = true;
    new_is_prime[0] = new_is_prime[1] = false;

    /* Standard Eratosthenes sieve */
    for (idx_t p = 2; (idx_t)p * p <= limit; ++p) {
        if (!new_is_prime[p]) continue;
        for (idx_t j = p * p; j <= limit; j += p)
            new_is_prime[j] = false;
    }

    /* Count primes and build table */
    idx_t count = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_is_prime[i]) ++count;

    idx_t *new_primes = (idx_t*)malloc(count * sizeof(idx_t));
    if (!new_primes) {
        free(new_is_prime);
        return;
    }

    idx_t k = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (new_is_prime[i]) new_primes[k++] = i;

    /* Free old storage and install new */
    free(is_prime);
    free(primes_arr);
    is_prime   = new_is_prime;
    primes_arr = new_primes;
    num_primes = count;
    sieve_limit = limit;
}

/* Ensure we have at least `n` primes stored.  Uses a generous upper bound
   to make room for the sum n+prime(n). */
static void ensure_sieve_for(idx_t n)
{
    if (n <= num_primes) return;           /* already sufficient */
    /* Estimate a limit that is guaranteed to contain `n` primes;
       add a small margin to cope with the n+prime(n) term */
    idx_t need_limit = estimate_prime_upper_bound(n + 200);
    if (need_limit <= sieve_limit) return; /* current sieve already big enough */
    rebuild_sieve(need_limit);
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* behaviour below offset irrelevant */

    ensure_sieve_for(n);
    if (n > num_primes) return 0;          /* safety net: should not happen */

    seq_t prime_n = (seq_t)primes_arr[n - 1];  /* p_n */
    seq_t sum     = (seq_t)n + prime_n;       /* n + p_n */

    /* Guarantee the sieve covers the range up to the next prime after sum. */
    if ((idx_t)(sum + 1) > sieve_limit) {
        idx_t new_limit = estimate_prime_upper_bound((idx_t)(sum + 1) + 200);
        rebuild_sieve(new_limit);
    }

    idx_t candidate = (idx_t)(sum + 1);
    while (candidate <= sieve_limit && !is_prime[candidate])
        ++candidate;

    /* candidate now holds the next prime > sum */
    return (seq_t)(candidate) - sum;
}
