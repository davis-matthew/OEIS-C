/*
 * A106769  Primes with digit sum = 34.
 * https://oeis.org/A106769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth prime with digit sum 34
 * Range:     u8 none, u16 n <= 60, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>


/* Helper: compute digit sum of an unsigned 64‑bit integer */
static uint64_t digit_sum_uint64(uint64_t x)
{
    uint64_t sum = 0;
    while (x > 0)
    {
        sum += x % 10;
        x /= 10;
    }
    return sum;
}

/* Helper: 64‑bit modular exponentiation */
static uint64_t modpow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t result = 1;
    while (e > 0)
    {
        if (e & 1ULL)
            result = (unsigned __int128)result * a % mod;
        a = (unsigned __int128)a * a % mod;
        e >>= 1ULL;
    }
    return result;
}

/* Helper: 64‑bit deterministic Miller–Rabin primality test */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small prime trial division */
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i)
    {
        uint64_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }

    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned int s = 0;
    while ((d & 1ULL) == 0ULL)
    {
        d >>= 1ULL;
        ++s;
    }

    /* deterministic bases for 64‑bit */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0ULL)
            continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1ULL || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned int r = 1; r < s; ++r)
        {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1)
            {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static state for cached primes */
    static uint64_t *primes = NULL;
    static idx_t   primes_cnt = 0;
    static idx_t   primes_cap = 0;
    static uint64_t last_checked = 1;

    /* Grow cache until it contains at least n primes */
    while (primes_cnt < n)
    {
        /* Find next candidate prime */
        do
        {
            ++last_checked;
            if ((last_checked & 1ULL) == 0ULL && last_checked != 2)
                continue;                       /* skip even numbers */
        } while (digit_sum_uint64(last_checked) != 34ULL || !is_prime_uint64(last_checked));

        /* Ensure capacity */
        if (primes_cnt == primes_cap)
        {
            idx_t new_cap = primes_cap == 0 ? 1 : primes_cap << 1;
            uint64_t *tmp = (uint64_t *)realloc(primes, new_cap * sizeof(uint64_t));
            if (tmp == NULL)
            {
                /* out of memory – abort gracefully */
                abort();
            }
            primes = tmp;
            primes_cap = new_cap;
        }

        /* Store found prime */
        primes[primes_cnt++] = last_checked;
    }

    /* Return the nth prime (1‑based) cast to seq_t */
    return (seq_t)primes[n - 1];
}
