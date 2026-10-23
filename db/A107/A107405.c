/*
 * A107405  Numbers n such that 109*n + 107 is prime.
 * https://oeis.org/A107405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = the n‑th integer n for which 109*n + 107 is prime (offset 1) */
/* Sequence: 0, 6, 18, 20, 44, 50, 56, 78, 98, 104, 110, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Deterministic Miller–Rabin primality test for 64‑bit integers */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2) return false;

    /* Trial division by small primes to short‑circuit composites */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
        19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0ULL) return false;
    }

    /* Write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;

        unsigned __int128 x = 1, p = a, exp = d;
        /* binary exponentiation: x = a^d mod n */
        while (exp > 0) {
            if (exp & 1ULL) x = (x * p) % n;
            p = (p * p) % n;
            exp >>= 1ULL;
        }
        if (x == 1 || x == n - 1) continue;

        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = (x * x) % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                   /* safety for out‑of‑bounds */

    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static uint64_t candidate = 0;          /* next n to test */

    size_t target = (size_t)n;              /* need first n terms (1‑based) */

    while (cache_size < target) {
        uint64_t val = 109ULL * candidate + 107ULL;
        if (is_prime_uint64(val)) {
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0) ? 1 : cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;       /* out of memory – return 0 as sentinel */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size] = (seq_t)candidate;
            ++cache_size;
        }
        ++candidate;
    }

    return cache[n - 1];
}
