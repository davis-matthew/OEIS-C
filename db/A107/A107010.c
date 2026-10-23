/*
 * A107010  Primes of the form 5x^2+xy+5y^2, with x and y any integer.
 * https://oeis.org/A107010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 1641, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A107010: primes of the form 5x² + xy + 5y² (discriminant –99) */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Integer square root of a 128‑bit unsigned integer, floor(sqrt(n)). */
static uint64_t isqrt_uint128(unsigned __int128 n)
{
    unsigned __int128 low = 0, high = 1;
    while (high * high <= n) high <<= 1;
    while (high > low + 1) {
        unsigned __int128 mid = (low + high) >> 1;
        if (mid * mid <= n)
            low = mid;
        else
            high = mid;
    }
    return (uint64_t)low;
}

/* Deterministic Miller–Rabin test for 64‑bit integers. */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] =
        {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint64_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] =
        {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        unsigned __int128 x = 1, base = a, exp = d;
        while (exp) {
            if (exp & 1) x = x * base % n;
            base = base * base % n;
            exp >>= 1;
        }
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (uint64_t r = 1; r < s; ++r) {
            x = x * x % n;
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Test whether the prime p can be written as 5x² + xy + 5y². */
static bool representable(uint64_t p)
{
    /* Loop over y with 20p – 99y² ≥ 0. */
    for (uint64_t y = 0;; ++y) {
        unsigned __int128 yy = (unsigned __int128)y * y;
        unsigned __int128 left  = (unsigned __int128)99 * yy; /* 99 y²  */
        unsigned __int128 right = (unsigned __int128)20 * p;   /* 20 p    */
        if (left > right) break;                 /* 20p – 99y² < 0 */
        unsigned __int128 D = right - left;      /* discriminant */
        uint64_t t = isqrt_uint128(D);
        if ((unsigned __int128)t * t == D) {
            uint64_t r10 = t % 10;
            uint64_t y10 = y % 10;
            if (r10 == y10 || r10 == (10 - y10) % 10) return true;
        }
    }
    return false;
}

/* Dynamic array to store discovered primes. */
static uint64_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

/* Ensure the cache can hold “needed” elements. */
static void ensure_cache_cap(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    uint64_t *new_ptr = (uint64_t *)realloc(cache, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    cache = new_ptr;
    cache_cap = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                  /* undefined below offset */
    static uint64_t candidate = 3;          /* start from the first odd prime */
    while (cache_size < (size_t)n) {
        if (isPrime64(candidate) && representable(candidate)) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = candidate;
        }
        candidate += 2;                    /* skip even numbers */
    }
    return (seq_t)cache[n - 1];             /* cast to seq_t */
}
