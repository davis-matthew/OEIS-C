/*
 * A103982  Indices of octahedral numbers (A005900) which are semiprimes.
 * https://oeis.org/A103982
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the nth index k for which the octahedral number (2*k³ +
 *            k)/3 is a semiprime (i.e. has exactly two prime factors, counted
 *            with multiplicity)
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* ----- 64‑bit deterministic Miller–Rabin primality test ----- */
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t mod_pow64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    a %= mod;
    while (e) {
        if (e & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static inline bool is_prime_ull(uint64_t n)
{
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases that make the test deterministic for 64‑bit integers */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* test whether k is in the sequence */
static bool is_semiprime_oct(idx_t k)
{
    if (k % 3 == 0) {
        uint64_t m = k / 3;
        if (!is_prime_ull(m)) return false;
        uint64_t val = (uint64_t)18 * m * m + 1;    /* (18*m² + 1) */
        return is_prime_ull(val);
    } else {
        if (!is_prime_ull(k)) return false;
        uint64_t val = ((uint64_t)2 * k * k + 1) / 3;  /* (2*k² + 1)/3 */
        return is_prime_ull(val);
    }
}

/* ----- sequence accessor ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, harmless */

    /* dynamic cache of found indices */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t cache_cap = 0;
    static idx_t next_k = 2;       /* start searching from k = 2 */

    /* initialise cache once */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until we have at least n terms */
    while (cache_len < n) {
        /* find the next qualifying index */
        while (!is_semiprime_oct(next_k))
            ++next_k;

        /* grow cache if needed */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* store the found index */
        cache[cache_len++] = (seq_t)next_k;
        ++next_k;
    }

    return cache[n - 1];
}
