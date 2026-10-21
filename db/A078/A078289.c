/*
 * A078289  Least nontrivial multiple of the n-th prime beginning with 5.
 * https://oeis.org/A078289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = least non‑trivial multiple of the n‑th prime * whose
 *            decimal representation starts with the digit 5
 * Range:     u8 n <= 8, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Static storage for primes */
static idx_t  *prime_list   = NULL;   /* array of the first primes */
static idx_t   prime_count  = 0;      /* number of primes stored   */

/* Ensure that prime_list contains at least n primes */
static void
ensure_primes(idx_t n)
{
    if (prime_count >= n)
        return;

    /* Estimate an upper bound for the n‑th prime */
    idx_t limit;
    if (n < 6)
        limit = 15;                 /* covers the first 6 primes */
    else
    {
        double dn = (double)n;
        limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 10.0);
    }

    /* Find a sieve size that yields at least n primes */
    while (true)
    {
        /* Sieve of Eratosthenes up to 'limit' */
        bool *is_prime = (bool *)malloc((limit + 1) * sizeof(bool));
        if (!is_prime)
            exit(1);                 /* out of memory */

        for (idx_t i = 0; i <= limit; ++i)
            is_prime[i] = true;
        if (limit >= 0) is_prime[0] = false;
        if (limit >= 1) is_prime[1] = false;

        for (idx_t i = 2; i * i <= limit; ++i)
        {
            if (is_prime[i])
                for (idx_t j = i * i; j <= limit; j += i)
                    is_prime[j] = false;
        }

        /* Count primes */
        idx_t count = 0;
        for (idx_t i = 2; i <= limit; ++i)
            if (is_prime[i])
                ++count;

        if (count >= n)
        {
            /* Allocate and fill prime_list */
            prime_list = (idx_t *)malloc(count * sizeof(idx_t));
            if (!prime_list)
                exit(1);
            idx_t pos = 0;
            for (idx_t i = 2; i <= limit; ++i)
                if (is_prime[i])
                    prime_list[pos++] = i;
            prime_count = count;
            free(is_prime);
            break;
        }

        /* Too few primes – double the limit and try again */
        free(is_prime);
        limit *= 2;
    }
}

/* Return the least non‑trivial multiple of the n‑th prime that
 * starts with digit 5.  n >= 1.  The function is memoised via
 * ensure_primes and works for any order of calls. */
seq_t a(idx_t n)
{
    ensure_primes(n);
    idx_t p = prime_list[n - 1];          /* nth prime (1‑based) */

    for (idx_t k = 2;; ++k)                /* non‑trivial multiples only */
    {
        uint64_t prod = (uint64_t)p * (uint64_t)k;

        /* Compute leading decimal digit of prod */
        uint64_t tmp = prod;
        while (tmp >= 10)
            tmp /= 10;
        if (tmp == 5)
            return (seq_t)prod;
    }
}
