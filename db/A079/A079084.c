/*
 * A079084  Greatest prime factor of (prime(n)+1)*(prime(n+1)+1)/4.
 * https://oeis.org/A079084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = greatest prime factor of (prime(n)+1)*(prime(n+1)+1)/4
 * Range:     u8 n <= 98, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <math.h>

/* ---------- Prime generation utilities ---------- */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static size_t sieve_limit = 0;

/* Ensure we have at least `need` primes in prime_list (1‑based). */
static void
ensure_primes(idx_t need)
{
    if (prime_cnt >= (size_t)need)
        return;

    /* Upper bound for the n‑th prime: p_n < n(ln n + ln ln n) for n >= 6 */
    size_t limit;
    if (need < 6)
        limit = 15;
    else
    {
        double n = (double) need;
        double approx = n * (log(n) + log(log(n)));
        limit = (size_t)(approx + 10.0);  /* a small safety margin */
    }
    /* Ensure the limit increases if we need more primes. */
    if (limit <= sieve_limit)
        limit = sieve_limit + 10;

    /* Allocate/combine the prime list to hold all primes up to `limit`. */
    prime_list = (uint64_t *)realloc(prime_list, limit * sizeof(uint64_t));
    /* Sieve of Eratosthenes */
    unsigned char *is_comp = (unsigned char *)calloc(limit + 1, 1);
    size_t count = 0;
    for (size_t i = 2; i <= limit && count < need; ++i)
    {
        if (!is_comp[i])
        {
            if (count < need)
                prime_list[count++] = i;
            if (i * i <= limit)
            {
                for (size_t j = i * i; j <= limit; j += i)
                    is_comp[j] = 1;
            }
        }
    }

    free(is_comp);
    prime_cnt   = count;
    sieve_limit = limit;
}

/* Return the k‑th prime (k >= 1). */
static uint64_t
nth_prime(idx_t k)
{
    ensure_primes(k);
    return prime_list[k - 1];
}

/* ---------- Greatest prime factor for 64‑bit integers ---------- */
static uint64_t
gpf_uint64(uint64_t n)
{
    if (n <= 1)
        return n;
    uint64_t largest = 0;
    for (size_t i = 0; i < prime_cnt && prime_list[i] * prime_list[i] <= n; ++i)
    {
        uint64_t p = prime_list[i];
        if (n % p == 0)
        {
            largest = p;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)
        largest = n;      /* remaining prime factor */
    return largest;
}

/* ---------- Main sequence function ---------- */
seq_t
a(idx_t n)
{
    /* We need the n‑th and (n+1)‑th primes. */
    ensure_primes(n + 1);
    uint64_t p1 = prime_list[n - 1];  /* prime(n) */
    uint64_t p2 = prime_list[n];      /* prime(n+1) */

    /* Compute ((p1+1)*(p2+1))/4.  The product is always divisible by 4. */
    uint64_t term = ((p1 + 1) * (p2 + 1)) / 4u;

    /* Return the greatest prime factor.  The value fits in 64 bits
       for all n up to at least 10000, so we can safely use the
       64‑bit helper.  In builds where seq_t is wider, the cast
       is harmless. */
    return (seq_t)gpf_uint64(term);
}
