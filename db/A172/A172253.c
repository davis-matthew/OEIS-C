/*
 * A172253  Numbers k such that the squarefree kernel of 9^k*(9^k - 1) is
 *          3*(9^k - 1)/4.
 * https://oeis.org/A172253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard
 * Formula:   not stated
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: k such that (9^k - 1)/4 is squarefree (equivalently,
   the squarefree kernel of 9^k*(9^k - 1) equals 3*(9^k - 1)/4). */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* modular exponentiation (base^exp mod mod) using 128‑bit mul */
static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (unsigned __int128)result * b % mod;
        b = (unsigned __int128)b * b % mod;
        exp >>= 1ULL;
    }
    return result;
}

/* multiplicative order of 9 modulo p^2 for odd prime p */
static uint64_t order_mod_p2(uint32_t p)
{
    uint64_t mod  = (uint64_t)p * (uint64_t)p;
    uint64_t order = (uint64_t)p * (uint64_t)(p - 1); /* φ(p²) */
    uint64_t t = order;

    /* factor φ(p²) by trial division */
    for (uint64_t q = 2; q * q <= t; ++q)
    {
        if (t % q == 0)
        {
            while (t % q == 0) t /= q;
            while (order % q == 0 &&
                   pow_mod64(9, order / q, mod) == 1ULL)
                order /= q;
        }
    }
    if (t > 1ULL)
    {
        uint64_t q = t;
        while (order % q == 0 &&
               pow_mod64(9, order / q, mod) == 1ULL)
            order /= q;
    }
    return order;
}

seq_t a(idx_t n)
{
    /* ---------- precomputation of prime orders ---------- */
    static bool ready = false;
    static size_t prime_cnt = 0;
    static const uint32_t MAX_PRIME = 100000;
    static uint32_t primes[9600];
    static uint64_t dvals[9600];

    if (!ready)
    {
        bool sieve[MAX_PRIME + 1];
        for (uint32_t i = 0; i <= MAX_PRIME; ++i) sieve[i] = true;
        sieve[0] = sieve[1] = false;
        for (uint32_t i = 2; i * i <= MAX_PRIME; ++i)
            if (sieve[i])
                for (uint32_t j = i * i; j <= MAX_PRIME; j += i)
                    sieve[j] = false;

        for (uint32_t i = 5; i <= MAX_PRIME; ++i)
        {
            if (!sieve[i]) continue;          /* skip composites          */
            primes[prime_cnt] = i;
            dvals[prime_cnt] = order_mod_p2(i);
            ++prime_cnt;
        }
        ready = true;
    }

    /* ---------- generate sequence until n-th term ---------- */
    idx_t count = 0;
    idx_t candidate = 1;

    while (true)
    {
        if ((candidate & 1ULL) != 0ULL)          /* only odd candidates */
        {
            bool bad = false;
            for (size_t i = 0; i < prime_cnt; ++i)
            {
                if (dvals[i] <= candidate &&
                    candidate % dvals[i] == 0ULL)
                {
                    bad = true;
                    break;
                }
            }
            if (!bad)
            {
                ++count;
                if (count == n)
                    return (seq_t)candidate;
            }
        }
        ++candidate;
    }
}
