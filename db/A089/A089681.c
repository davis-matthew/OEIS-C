/*
 * A089681  Numbers n such that 3n^2 - 1 is prime.
 * https://oeis.org/A089681
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that 3*n*n - 1 is prime.
   a(n) returns the nth such n (offset 1). */

#include <stdint.h>
#include <stdlib.h>

/* ------- 64‑bit deterministic Miller–Rabin primality test for n < 2^64 ------- */

static uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((__uint128_t)a * b % m);
}

static uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t m)
{
    uint64_t res = 1;
    a %= m;
    while (e)
    {
        if (e & 1) res = mod_mul64(res, a, m);
        a = mod_mul64(a, a, m);
        e >>= 1;
    }
    return res;
}

static int is_prime64(uint64_t n)
{
    if (n < 2) return 0;
    /* small primes */
    static const uint64_t small[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL
    };
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i)
    {
        if (n == small[i]) return 1;
        if (n % small[i] == 0) return 0;
    }
    /* write n-1 = d * 2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1ULL) == 0) { d >>= 1; ++s; }
    /* deterministic bases for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        int comp = 1;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul64(x, x, n);
            if (x == n - 1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/* ------- Memoized sequence storage ------- */
static seq_t *a_cache = NULL;
static idx_t a_cache_len = 0;
static idx_t a_cache_cap = 0;

/* Ensure cache can hold at least 'nt' terms. */
static void a_cache_ensure(idx_t nt)
{
    if (nt <= a_cache_len) return;
    if (a_cache_cap < nt)
    {
        idx_t new_cap = a_cache_cap ? a_cache_cap * 2 : 8;
        while (new_cap < nt) new_cap <<= 1;
        seq_t *newptr = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        if (!newptr) abort();
        a_cache = newptr;
        a_cache_cap = new_cap;
    }
    /* Compute missing terms. */
    idx_t cand = (a_cache_len == 0) ? (idx_t)1 : (idx_t)(a_cache[a_cache_len - 1] + 1);
    while (a_cache_len < nt)
    {
        /* 3*n*n - 1 fits in 128 bits; cast to 64 bits for primality test. */
        unsigned __int128 val128 = (unsigned __int128)3 * cand * cand - 1;
        if (is_prime64((uint64_t)val128))
            a_cache[a_cache_len++] = cand;
        ++cand;
    }
}

/* ------------------------------------------------------------ */
/* seq_t a(idx_t n)  returns the nth number with 3n^2-1 prime   */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for offset‑1 */
    a_cache_ensure(n);
    return a_cache[n - 1];
}
