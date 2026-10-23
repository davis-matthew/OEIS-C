/*
 * A107201  Primes of the form 8x^2 + 11y^2.
 * https://oeis.org/A107201
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 815, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime of the form 8*x^2 + 11*y^2
 * (with x,y integers, y≥0).  11 is included as the first term.
 *
 * The function uses a cache of found terms so that subsequent calls
 * are O(1).  Each term is obtained by testing odd candidates for
 * primality and the quadratic‑form representation.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod64.h"

/*--- Integer square root for 64‑bit values --------------------------------*/
static uint64_t int_sqrt(uint64_t v)
{
    uint64_t lo = 0, hi = (1ULL << 32);   /* hi > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid <= v / mid)   lo = mid;
        else                  hi = mid - 1;
    }
    return lo;
}

/*--- 64‑bit modular multiplication --------------------------------------*/

/*--- 64‑bit modular exponentiation ------------------------------------*/
static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1) res = mulmod64(res, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/*--- Deterministic Miller–Rabin primality test for 64‑bit integers ------*/
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;
    /* small prime trial division */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* deterministic bases for 2^64 range */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/*--- Check whether a prime p can be written as 8*x^2 + 11*y^2 -----------*/
static bool has_representation(uint64_t p)
{
    uint64_t y_limit = int_sqrt(p / 11);
    for (uint64_t y = 0; y <= y_limit; ++y) {
        unsigned __int128 rem = (unsigned __int128)p - (unsigned __int128)11 * y * y;
        if (rem == 0) return true;     /* 11*1^2 or 8*x^2 term zero */
        if ((rem & 7u) != 0) continue; /* need rem % 8 == 0 */
        uint64_t x2 = (uint64_t)(rem >> 3);   /* rem / 8 */
        uint64_t x  = int_sqrt(x2);
        if ((unsigned __int128)x * x == x2) return true;
    }
    return false;
}

/*--- Main sequence function ----------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;             /* safety for out‑of‑range index */

    /* Static cache of found terms */
    static uint64_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_cap   = 0;

    /* initialise cache with the first term (11) */
    if (cache_size == 0) {
        cache_cap = 16;
        cache = (uint64_t*)malloc(cache_cap * sizeof(uint64_t));
        cache[0] = 11;   /* a(1) = 11 */
        cache_size = 1;
    }

    /* extend cache until it contains at least n terms */
    if (cache_size < n) {
        uint64_t candidate = cache[cache_size - 1] + 2;   /* start after last found */
        if (candidate <= 2) candidate = 3;  /* in case last was 11 -> start 13 */

        while (cache_size < n) {
            if (candidate < 11) candidate = 11;   /* ensure we start from 11 */

            if (candidate >= 11) {
                if (candidate % 2 == 0) {   /* skip even numbers */
                    candidate += 1;
                    continue;
                }
                if (is_prime_uint64(candidate)) {
                    uint8_t r = (uint8_t)(candidate % 88);
                    if (r == 19 || r == 35 || r == 43 || r == 51 || r == 83) {
                        if (has_representation(candidate)) {
                            /* grow cache if needed */
                            if (cache_size == cache_cap) {
                                cache_cap <<= 1;
                                cache = (uint64_t*)realloc(cache,
                                                           cache_cap * sizeof(uint64_t));
                            }
                            cache[cache_size++] = candidate;
                        }
                    }
                }
            }
            candidate += 2;   /* next odd candidate */
        }
    }

    return (seq_t)cache[n - 1];
}
