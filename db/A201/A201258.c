/*
 * A201258  Primes of the form 7*n^3 + 10.
 * https://oeis.org/A201258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 68, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Validated: standard, pass, 42 terms through n = 2928 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A201258: Primes of the form 7*n^3 + 10.
   a(n) is the n-th such prime (n ≥ 1). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

static inline unsigned bitlen_seq_t(seq_t x) {
    if (x == 0) return 0;
    unsigned bits = 0;
    while (x) {
        x >>= 1;
        ++bits;
    }
    return bits;
}

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

static inline uint64_t _mod_mul(uint64_t a, uint64_t b, uint64_t m)
{
#if defined(__SIZEOF_INT128__)
    return (uint64_t)((__uint128_t)a * b % m);
#else
    uint64_t res = 0;
    while (b) {
        if (b & 1) res = (res + a) % m;
        a = (a << 1) % m;
        b >>= 1;
    }
    return res;
#endif
}

static inline uint64_t _pow_mod(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = _mod_mul(result, base, mod);
        base = _mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool _is_prime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n == small[i]) return true;
        if (n % small[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = _pow_mod(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = _mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* 64‑bit Miller–Rabin primality test ------------------------------------------------- */

static bool _is_prime_seq(seq_t n)
{
    if (bitlen_seq_t(n) <= 64)
        return _is_prime64((uint64_t)n);
    return is_prime_big(n);
}

/* Cache of found primes ------------------------------------------------------------ */

static seq_t *prime_cache = NULL;   /* holds primes of the form 7*k^3+10 */
static size_t   cache_len   = 0;     /* number of cached primes                */
static size_t   cache_cap   = 0;     /* allocated capacity                      */
static idx_t    next_k      = 1;     /* next k to test                         */

static void _ensure_capacity(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 8 : cache_cap << 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
    if (!tmp) abort();
    prime_cache = tmp;
    cache_cap = new_cap;
}

static void _append_prime(seq_t p)
{
    _ensure_capacity(cache_len + 1);
    prime_cache[cache_len++] = p;
}

static void _generate_up_to(idx_t n)
{
    while (cache_len < n) {
        seq_t k  = (seq_t)next_k;
        seq_t k2 = k * k;
        seq_t k3 = k2 * k;
        seq_t p  = k3 * (seq_t)7 + (seq_t)10;
        if (_is_prime_seq(p)) _append_prime(p);
        ++next_k;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, but safe */
    _generate_up_to(n);
    return prime_cache[n - 1];
}
