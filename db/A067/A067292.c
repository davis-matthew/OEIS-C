/*
 * A067292  a(n) = prime(n)-n*tau(n) where tau(n) is the number of divisors of
 *          n.
 * https://oeis.org/A067292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = prime(n) - n * τ(n)
 * Range:     i8 n <= 35, i16 n <= 70, i32 n <= 70, i64 n <= 70, i128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where τ(n) is the number of divisors of n. */

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/* ---- Dynamic sieve state ---- */
static bool   *sieve_composite = NULL;   /* array marking composites      */
static idx_t   sieve_limit     = 0;       /* current sieve limit           */
static idx_t *prime_list      = NULL;     /* list of discovered primes     */
static size_t prime_count     = 0;        /* number of primes stored       */

/* ---- Ensure the sieve contains at least `need` primes ---- */
static void ensure_primes(idx_t need)
{
    if (prime_count >= (size_t)need)
        return;   /* already sufficient */

    /* Estimate an upper bound for the `need`‑th prime. */
    idx_t new_limit;
    if (need < 6)
        new_limit = 15;
    else {
        double d = (double)need;
        new_limit = (idx_t)(d * (log(d) + log(log(d)))) + 3;
    }

    /* Allocate and initialize new sieve. */
    bool *new_sieve = (bool*)malloc((new_limit + 1) * sizeof(bool));
    if (!new_sieve)  /* out of memory: abort silently */
        return;
    for (idx_t i = 0; i <= new_limit; ++i)
        new_sieve[i] = false;
    new_sieve[0] = new_sieve[1] = true;   /* 0 and 1 are composite */

    /* Sieve of Eratosthenes. */
    for (idx_t i = 2; i * i <= new_limit; ++i)
        if (!new_sieve[i])
            for (idx_t j = i * i; j <= new_limit; j += i)
                new_sieve[j] = true;

    /* Count primes. */
    size_t cnt = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_sieve[i])
            ++cnt;

    /* Allocate prime list. */
    idx_t *new_primes = (idx_t*)malloc(cnt * sizeof(idx_t));
    if (!new_primes) {
        free(new_sieve);
        return;
    }

    /* Store primes. */
    size_t pos = 0;
    for (idx_t i = 2; i <= new_limit; ++i)
        if (!new_sieve[i])
            new_primes[pos++] = i;

    /* Replace old structures. */
    free(sieve_composite);
    free(prime_list);
    sieve_composite = new_sieve;
    prime_list      = new_primes;
    prime_count     = cnt;
    sieve_limit     = new_limit;
}

/* ---- Count divisors of a 64‑bit integer n. ---- */
static seq_t _divisor_count_u64(idx_t n)
{
    seq_t tau = 1;
    idx_t m   = n;

    /* Ensure some primes are available for factorisation. */
    ensure_primes(6);   /* 6 is minimal safe value */

    for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= m; ++i) {
        idx_t p = prime_list[i];
        if (m % p == 0) {
            unsigned exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            tau *= (exp + 1);
        }
    }
    if (m > 1)          /* remaining prime factor */
        tau *= 2;

    return tau;
}

/* ---- Main sequence function: a(n) = prime(n) - n * τ(n) ---- */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined offset; safe default */
        return (seq_t)0;

    ensure_primes(n);                /* Need at least n primes. */
    seq_t prime_n = (seq_t)prime_list[n - 1];   /* nth prime, offset 1 */
    seq_t tau_n   = _divisor_count_u64(n);      /* τ(n) */
    seq_t n_seq   = (seq_t)n;

    return prime_n - n_seq * tau_n;
}
