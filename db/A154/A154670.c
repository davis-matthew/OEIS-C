/*
 * A154670  Averages of twin prime pairs k such that k*2 and k/2 are squares.
 * https://oeis.org/A154670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 18*A037073(n)^2. Computes a(n) by searching for integers
 *            k where 18*k^2-1 and 18*k^2+1 are both primes
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 620, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ----- fast 64‑bit modular arithmetic ----- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t result = 1 % mod;
    a %= mod;
    while (e > 0) {
        if (e & 1)
            result = mul_mod64(result, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return result;
}

/* ----- deterministic Miller–Rabin for 64‑bit integers ----- */
static bool is_prime_64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,
                                     23ULL,29ULL,31ULL,37ULL};
    size_t i;
    for (i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0)
            return n == small[i];
    }

    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* deterministic bases for 64‑bit numbers */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    size_t j;
    for (j = 0; j < sizeof(bases)/sizeof(bases[0]); ++j) {
        uint64_t a = bases[j] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        int r;
        for (r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1)
                break;
        }
        if (r == s)
            return false;
    }
    return true;
}

/* ----- memoised sequence generator ----- */
static seq_t *cache   = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static uint64_t last_t   = 0; /* last t that was examined */

static void ensure_cache_size(idx_t target)
{
    if ((size_t)target <= cache_len) return;

    while (cache_len < (size_t)target) {
        /* Find next t giving twin primes for 18*t^2 */
        while (1) {
            ++last_t;
            uint64_t t2 = last_t * last_t;
            uint64_t base = 18ULL * t2;
            uint64_t p1 = base - 1;
            uint64_t p2 = base + 1;
            if (is_prime_64(p1) && is_prime_64(p2)) {
                /* store value a = 18*t^2 in seq_t */
                seq_t val = (seq_t)18 * (seq_t)last_t * (seq_t)last_t;
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 16;
                    seq_t *newptr = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (newptr == NULL) abort();
                    cache = newptr;
                }
                cache[cache_len++] = val;
                break;
            }
        }
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* undefined for n < 1 */
    ensure_cache_size(n);
    return cache[n - 1];
}
