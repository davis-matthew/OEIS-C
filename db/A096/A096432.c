/*
 * A096432  Let n = 2^e_2 * 3^e_3 * 5^e_5 * ... be the prime factorization of
 *          n; sequence gives n such that 1 + max{e_2, e_3, ...} is a prime.
 * https://oeis.org/A096432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A096432
   a(n) is the n-th positive integer n such that 1 + max{e_p} is prime,
   where n = ∏p^e_p is the prime factorisation of n. */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* test whether x is prime for small x (x <= 64) */
static bool isPrimeSmall(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint64_t i = 3; i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

/* generate the sequence lazily */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cache_len = 0;  /* number of terms cached */
    static size_t cache_cap = 0;  /* allocated capacity of cache */
    static uint64_t next_candidate = 2; /* next integer to test */

    while (cache_len < n)
    {
        uint64_t cand = next_candidate++;
        uint64_t temp = cand;
        uint64_t max_exp = 0;

        /* factorise cand by trial division and find the maximum exponent */
        for (uint64_t p = 2; p * p <= temp; ++p)
        {
            if (temp % p == 0)
            {
                uint64_t e = 0;
                while (temp % p == 0)
                {
                    temp /= p;
                    ++e;
                }
                if (e > max_exp) max_exp = e;
            }
        }
        if (temp > 1)           /* remaining prime factor */
            if (max_exp < 1) max_exp = 1;

        /* test condition */
        if (isPrimeSmall(max_exp + 1))
        {
            /* grow cache if needed */
            if (cache_len + 1 > cache_cap)
            {
                cache_cap = cache_cap ? cache_cap * 2 : 128;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)cand;
        }
    }
    return cache[n - 1];
}
