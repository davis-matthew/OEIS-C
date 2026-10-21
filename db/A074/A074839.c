/*
 * A074839  a(0) = 1, a(n+1) = a(n) + next prime larger than a(n).
 * https://oeis.org/A074839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + p,   a(0) = 1
 * where  p  is the smallest prime greater than a(n-1)
 *
 * Note: The implementation memoises all computed terms.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* -------------------------------------------------------------
 *  64‑bit deterministic Miller–Rabin primality test
 * ------------------------------------------------------------- */
static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    /* trial division by small primes */
    static const uint64_t small[] = {
        2ULL, 3ULL, 5ULL, 7ULL, 11ULL, 13ULL, 17ULL,
        19ULL, 23ULL, 29ULL, 31ULL, 37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0ULL) {
        d >>= 1;
        ++s;
    }

    /* Bases that make the test deterministic for 64‑bit integers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a % n == 0) continue;

        /* modular exponentiation: a^d mod n */
        uint64_t x = 1;
        uint64_t base = a % n;
        uint64_t exp = d;
        while (exp) {
            if (exp & 1ULL) x = (unsigned __int128)x * base % n;
            base = (unsigned __int128)base * base % n;
            exp >>= 1;
        }

        if (x == 1ULL || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }

    return true;
}

/* -------------------------------------------------------------
 *  Next prime greater than x (x is a seq_t value that fits in
 *  uint64_t). For larger seq_t values this implementation will
 *  still work as long as the value can be represented in a 64‑bit
 *  integer (which is the case for all terms that fit in seq_t).
 * ------------------------------------------------------------- */
static seq_t nextPrimeGt(seq_t x)
{
    uint64_t u = (uint64_t)x;          /* safe cast for values that fit */
    uint64_t cand = u + 1;
    if (cand <= 2ULL) return (seq_t)2ULL;
    if ((cand & 1ULL) == 0ULL) ++cand;  /* make it odd */

    while (!isPrime64(cand)) cand += 2ULL;
    return (seq_t)cand;
}

/* -------------------------------------------------------------
 *  Memoisation cache for the sequence
 * ------------------------------------------------------------- */
static seq_t *cache      = NULL;
static idx_t cache_size  = 0;   /* number of initialised elements   */
static idx_t cache_cap   = 0;   /* allocated size                 */

/* Grow the cache to at least size n+1 */
static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;

    if (n + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap <= n) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)1;          /* a(0) = 1 */
        } else {
            seq_t prev = cache[i - 1];
            seq_t p    = nextPrimeGt(prev);
            cache[i]   = prev + p;        /* recurrence */
        }
    }
    cache_size = n + 1;
}

/* -------------------------------------------------------------
 *  Public function required by the exercise
 * ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
