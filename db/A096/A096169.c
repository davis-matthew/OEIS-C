/*
 * A096169  Odd n such that (n^4+1)/2 is prime.
 * https://oeis.org/A096169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: odd integers n such that (n^4+1)/2 is a prime number.
   a(n) = the nth odd integer n for which (n^4+1)/2 is prime. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* ---------- Miller-Rabin primality test for 128‑bit numbers ---------- */

static unsigned __int128 mul_mod128(unsigned __int128 a, unsigned __int128 b,
                                    unsigned __int128 mod)
{
    /* Russian‑peasant multiplication modulo mod. */
    unsigned __int128 res = 0;
    a %= mod;
    while (b) {
        if (b & 1)
            res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

static unsigned __int128 pow_mod128(unsigned __int128 a,
                                    unsigned __int128 exp,
                                    unsigned __int128 mod)
{
    unsigned __int128 result = 1;
    a %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod128(result, a, mod);
        a = mul_mod128(a, a, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller‑Rabin for unsigned __int128 (valid for all values). */
static bool is_prime128(unsigned __int128 n)
{
    if (n < 2)
        return false;
    /* small primes */
    static const unsigned long long small_primes[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL, 19ULL, 23ULL, 29ULL,
        31ULL, 37ULL, 41ULL, 43ULL, 47ULL, 53ULL, 59ULL, 61ULL, 67ULL,
        71ULL, 73ULL, 79ULL, 83ULL, 89ULL, 97ULL
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if ((unsigned __int128)small_primes[i] == n)
            return true;
        if (n % small_primes[i] == 0)
            return false;
    }
    /* write n-1 as d*2^s */
    unsigned __int128 d = n - 1;
    unsigned int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* test a few bases */
    static const unsigned int bases[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        unsigned __int128 a = bases[i] % n;
        if (a == 0)
            continue;
        unsigned __int128 x = pow_mod128(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool cont = false;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod128(x, x, n);
            if (x == n - 1) {
                cont = true;
                break;
            }
        }
        if (!cont)
            return false;
    }
    return true;
}

/* ---------- Main sequence function ---------- */

seq_t a(idx_t n)
{
    /* Memoization: cache the found odd integers */
    static seq_t *cache        = NULL;
    static size_t cache_len    = 0;
    static size_t cache_cap    = 0;
    static idx_t next_odd = 3;          /* next odd number to test */

    /* Expand cache until we have at least n terms */
    while (cache_len < n) {
        /* Grow storage if necessary */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute (next_odd^4 + 1)/2 as a 128‑bit value */
        unsigned __int128 t = (unsigned __int128)next_odd;
        t = t * t * t * t + 1;           /* next_odd^4 + 1 */
        unsigned __int128 candidate = t >> 1; /* (next_odd^4 + 1)/2 */

        if (is_prime128(candidate)) {
            cache[cache_len++] = (seq_t)next_odd;   /* store the odd integer itself */
        }
        next_odd += 2;  /* advance to next odd number */
    }

    return cache[n - 1];
}
