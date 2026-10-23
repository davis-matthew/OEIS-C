/*
 * A175218  The third nonprimes after the primes.
 * https://oeis.org/A175218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) + 2 + A010051(prime(n)+2) + 1
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>

/* Static storage for the prime sieve */
static idx_t   *primes      = NULL;   /* array of primes */
static size_t  primes_cnt   = 0;      /* number of primes stored */
static idx_t   sieve_limit  = 0;      /* largest number sieved so far */
static unsigned char *sieve = NULL;   /* 0 = prime, 1 = composite */

/* ------------------------------------------------------------------ */
/* Expand the sieve so that at least n primes are available.           */
/* ------------------------------------------------------------------ */
static void ensure_primes_up_to(idx_t n)
{
    while (primes_cnt < (size_t)n) {
        idx_t new_limit = (sieve_limit == 0) ? 100 : sieve_limit * 2;

        /* Allocate and zero the composite flag array. */
        unsigned char *tmp = (unsigned char *)calloc((size_t)new_limit + 1, 1);
        if (!tmp) exit(1);                   /* allocation failure */

        /* Sieve of Eratosthenes. */
        for (idx_t i = 2; i <= new_limit; ++i) {
            if (!tmp[i]) {
                if (i * i <= new_limit) {
                    for (idx_t j = i * i; j <= new_limit; j += i)
                        tmp[j] = 1;
                }
            }
        }

        /* Count primes within the new limit. */
        size_t cnt = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (!tmp[i])
                ++cnt;

        /* Store primes in a separate array. */
        idx_t *p_arr = (idx_t *)malloc(cnt * sizeof(idx_t));
        if (!p_arr) exit(1);
        size_t idx = 0;
        for (idx_t i = 2; i <= new_limit; ++i)
            if (!tmp[i])
                p_arr[idx++] = i;

        /* Replace old data with the new sieve. */
        free(primes);
        free(sieve);
        primes      = p_arr;
        primes_cnt  = cnt;
        sieve_limit = new_limit;
        sieve       = tmp;
    }
}

/* ------------------------------------------------------------------ */
/* Return true if v is a prime number.                                 */
/* Uses the sieve for numbers ≤ sieve_limit, otherwise trial division. */
/* ------------------------------------------------------------------ */
static bool is_prime_val(idx_t v)
{
    if (v <= sieve_limit)
        return !sieve[v];

    /* Trial division by all primes up to √v. */
    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p * p > v)
            break;
        if (v % p == 0)
            return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* Sequence a(n): the third non‑prime > the nth prime.                */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined, but harmless */

    ensure_primes_up_to(n);

    idx_t p     = primes[n - 1];    /* the nth prime */
    idx_t count = 0;

    for (idx_t x = p + 1; ; ++x) {
        if (!is_prime_val(x)) {
            ++count;
            if (count == 3)
                return (seq_t)x;
        }
    }
}
