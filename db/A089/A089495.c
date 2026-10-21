/*
 * A089495  a(n) = mu(prime(n)+1), where mu is the Moebius function.
 * https://oeis.org/A089495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = mu(prime(n)+1)
 * Range:     i8 n <= 118, i16 n <= 118, i32 n <= 118, i64 n <= 118,
 *            i128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Dynamic storage of primes. */
static uint64_t *prime_list = NULL;   /* array of primes */
static size_t    prime_cnt   = 0;      /* current number of primes stored */
static size_t    prime_cap   = 0;      /* allocated capacity */
static uint64_t  candidate   = 2;      /* next integer to test for primality */

/* Ensure that at least the first 'n' primes are computed and stored. */
static void
ensure_nth_prime(idx_t n)
{
    while (prime_cnt < n)
    {
        bool is_prime = true;

        /* Trial divide candidate by all known primes up to sqrt(candidate). */
        for (size_t i = 0; i < prime_cnt && prime_list[i] <= candidate / prime_list[i]; ++i)
        {
            if (candidate % prime_list[i] == 0)
            {
                is_prime = false;
                break;
            }
        }

        if (is_prime)
        {
            /* Grow storage if needed. */
            if (prime_cnt == prime_cap)
            {
                size_t new_cap = prime_cap ? prime_cap + 1024 : 1024;
                uint64_t *tmp = (uint64_t *)realloc(prime_list,
                                                  new_cap * sizeof(uint64_t));
                prime_list = tmp;
                prime_cap  = new_cap;
            }
            prime_list[prime_cnt++] = candidate;
        }
        ++candidate;
    }
}

/* Compute the Möbius function μ(x) using the stored primes. */
static int
mu_of_uint64(uint64_t x)
{
    int prime_factors = 0;

    for (size_t i = 0; i < prime_cnt && prime_list[i] <= x / prime_list[i]; ++i)
    {
        if (x % prime_list[i] == 0)
        {
            int exp = 0;
            while (x % prime_list[i] == 0)
            {
                ++exp;
                x /= prime_list[i];
                if (exp > 1)          /* square factor → μ = 0 */
                    return 0;
            }
            ++prime_factors;
        }
    }

    if (x > 1)        /* remaining prime factor */
        ++prime_factors;

    return (prime_factors % 2 == 0) ? 1 : -1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for n < 1, return 0 */

    ensure_nth_prime(n);

    /* nth prime (1‑based) */
    uint64_t p = prime_list[n - 1];

    /* μ(prime(n)+1) */
    uint64_t m = p + 1;
    int mu_val = mu_of_uint64(m);

    return (seq_t)mu_val;
}
