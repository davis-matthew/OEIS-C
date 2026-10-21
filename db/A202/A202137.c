/*
 * A202137  Numbers k such that 24k + 1 is neither square nor prime.
 * https://oeis.org/A202137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 139, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   intSqrt, isPerfectSquare, mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th integer k such that 24*k+1 is neither
 * prime nor a perfect square.  Indexing starts at 1.  The function
 * searches sequentially for valid k and memoises results so that calls
 * in arbitrary order are fast.  */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/intSqrt.h"
#include "../utils/isPerfectSquare.h"
#include "../utils/mod_mul.h"

/* ---------------------  Miller–Rabin 64‑bit  --------------------- */

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* Trial division by small primes */
    static const uint32_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    /* Miller–Rabin deterministic for 64‑bit */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t test_bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(test_bases)/sizeof(test_bases[0]); ++i) {
        uint64_t a = test_bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------------------  Memoised storage  ----------------------- */
#ifdef __cplusplus
static seq_t *cache      = nullptr;
static size_t cache_used = 0;
static size_t cache_cap  = 0;
static idx_t tested_k    = 0;

static void cache_grow(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    seq_t *new_cache = new seq_t[new_cap];
    if (cache) {
        for (size_t i = 0; i < cache_used; ++i) new_cache[i] = cache[i];
        delete [] cache;
    }
    cache       = new_cache;
    cache_cap   = new_cap;
}
#else
static seq_t cache[16384];
static size_t cache_used = 0;
static idx_t tested_k    = 0;
#endif

/* ---------------------  Public function  ------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1  */

#ifdef __cplusplus
    /* C++ implementation – dynamic array with growth */
    while (cache_used < (size_t)n) {
        ++tested_k;
        uint64_t k = (uint64_t)tested_k;
        uint64_t m = 24ULL * k + 1;
        if (!isPrime64(m) && !isPerfectSquare(m)) {
            if (cache_used == cache_cap) cache_grow();
            cache[cache_used++] = (seq_t)k;
        }
    }
    return cache[n-1];
#else
    /* C implementation – static array of fixed size */
    while (cache_used < (size_t)n) {
        ++tested_k;
        uint64_t k = (uint64_t)tested_k;
        uint64_t m = 24ULL * k + 1;
        if (!isPrime64(m) && !isPerfectSquare(m)) {
            cache[cache_used++] = (seq_t)k;
        }
    }
    return cache[n-1];
#endif
}
