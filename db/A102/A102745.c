/*
 * A102745  Number of distinct prime factors of four consecutively
 *          concatenated primes.
 * https://oeis.org/A102745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct prime factors of
 *            p_n||p_{n+1}||p_{n+2}||p_{n+3}
 * Range:     u8 n <= 51, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mul_mod_128, pow_mod_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where p_k is the k-th prime and || denotes decimal concatenation. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/mul_mod_128.h"
#include "../utils/pow_mod_128.h"

static inline bool is_probable_prime_128(__int128 n)
{
    if (n < 2) return false;
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    static const unsigned int bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31};
    int s = 0;
    __int128 d = n - 1;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        unsigned int a_uint = bases[i];
        if (a_uint >= n) break;
        __int128 a = a_uint;
        __int128 x = pow_mod_128(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod_128(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static inline __int128 gcd128(__int128 a, __int128 b)
{
    while (b != 0)
    {
        __int128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static inline __int128 pollard_rho_128(__int128 n)
{
    if ((n & 1) == 0) return 2;
    static __int128 seed = 1;
    __int128 c = seed++;
    __int128 x = 2, y = 2, d = 1;

    while (d == 1)
    {
        x = (mul_mod_128(x, x, n) + c) % n;
        y = (mul_mod_128(y, y, n) + c) % n;
        y = (mul_mod_128(y, y, n) + c) % n;
        __int128 diff = x > y ? x - y : y - x;
        d = gcd128(diff, n);
        if (d == n) return pollard_rho_128(n);
    }
    return d;
}

/* Recursively factorizes n into distinct prime factors stored in fac[0..*cnt-1] */
static void factor_128(__int128 n, __int128 *fac, size_t *cnt)
{
    if (n == 1) return;
    if (is_probable_prime_128(n))
    {
        fac[(*cnt)++] = n;
        return;
    }
    __int128 d = pollard_rho_128(n);
    factor_128(d, fac, cnt);
    factor_128(n / d, fac, cnt);
}

static inline unsigned int digit_count_uint64(uint64_t x)
{
    unsigned int c = 0;
    do { ++c; x /= 10; } while (x);
    return c;
}

seq_t a(idx_t n)
{
    /* Concatenate the four consecutive primes starting at index n */
    __int128 num = 0;
    for (idx_t i = 0; i < 4; ++i)
    {
        seq_t p_raw = get_prime((uint64_t)(n + i));
        uint64_t p = (uint64_t)p_raw;
        unsigned int d = digit_count_uint64(p);
        __int128 pow10 = 1;
        for (unsigned int j = 0; j < d; ++j) pow10 *= 10;
        num = num * pow10 + (__int128)p;
    }

    /* Factor num and count distinct prime factors */
    __int128 factors[32];
    size_t fac_cnt = 0;
    factor_128(num, factors, &fac_cnt);

    /* Count unique factors */
    size_t unique = 0;
    for (size_t i = 0; i < fac_cnt; ++i)
    {
        bool seen = false;
        for (size_t j = 0; j < unique; ++j)
            if (factors[i] == factors[j]) { seen = true; break; }
        if (!seen) factors[unique++] = factors[i];
    }

    return (seq_t)unique;
}
