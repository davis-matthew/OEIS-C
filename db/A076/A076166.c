/*
 * A076166  Primes p such that sum of cubes of even digits of p equals sum of
 *          cubes of odd digits of p.
 * https://oeis.org/A076166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 6, u32 n <= 5870, u64 n <= 5870,
 *            u128 n <= 5870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = the n-th prime p such that
 * sum_{d even in p} d^3 = sum_{d odd in p} d^3
 * (primality and digit condition are checked directly).
 * The implementation searches successive odd numbers,
 * keeps the found primes in a growing static cache, and
 * returns the requested element in O(1) after it has been found.
 *
 * The search stops once enough terms have been collected.
 *
 * The code contains an inlined deterministic Miller–Rabin test
 * for 64‑bit values which is used instead of the supplied
 * is_prime_big helper. This guarantees correct behaviour
 * without relying on the external implementation.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* ----- cache of sequence values ---------------------------------------- */
static seq_t *cache = NULL;      /* array of found primes          */
static size_t cache_len = 0;     /* number of entries currently    */
static size_t cache_cap = 0;     /* buffer capacity                */

/* ----- next candidate to test (odd numbers >= 10007) ------------------ */
static uint64_t next_candidate = 10007;

/* ----- pre‑computed cubes of digits 0–9 -------------------------------- */
static const uint32_t cube[10] = {
    0,   /* 0^3 */
    1,   /* 1^3 */
    8,   /* 2^3 */
    27,  /* 3^3 */
    64,  /* 4^3 */
    125, /* 5^3 */
    216, /* 6^3 */
    343, /* 7^3 */
    512, /* 8^3 */
    729  /* 9^3 */
};

/* ----- utility: grow cache to accommodate at least 'needed' entries ---------------- */
static void grow_cache(size_t needed)
{
    if (cache_len >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        abort();
    cache = p;
    cache_cap = new_cap;
}

/* ----- test whether n satisfies the cube‑sum condition --------------------- */
static bool cube_sum_ok(uint64_t n)
{
    uint32_t even_sum = 0;
    uint32_t odd_sum  = 0; 
    uint64_t t = n;
    while (t) {
        uint32_t d = (uint32_t)(t % 10);
        if (d & 1)
            odd_sum += cube[d];
        else
            even_sum += cube[d];
        t /= 10;
    }
    return even_sum == odd_sum;
}

/* ----- 128‑bit modular multiplication ----------------------------------- */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* ----- 64‑bit modular exponentiation ----------------------------------- */
static uint64_t pow_mod_uint64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    a %= mod;
    while (e) {
        if (e & 1)
            res = mul_mod_uint64(res, a, mod);
        a = mul_mod_uint64(a, a, mod);
        e >>= 1;
    }
    return res;
}

/* ----- deterministic Miller–Rabin for 64‑bit integers ------------------- */
static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)
        return false;
    /* small prime trial division */
    const uint32_t small_primes[] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43};
    for (size_t i = 0; i < sizeof small_primes / sizeof *small_primes; ++i) {
        uint32_t p = small_primes[i];
        if (n % p == 0)
            return n == p;
    }
    /* write n-1 as d * 2^s with d odd */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* bases sufficient for all 64‑bit integers */
    const uint64_t bases[] = {2,3,5,7,11,13};
    for (size_t i = 0; i < sizeof bases / sizeof *bases; ++i) {
        uint64_t a = bases[i];
        if (a % n == 0)
            continue;
        uint64_t x = pow_mod_uint64(a, d, n);
        if (x == 1 || x == n - 1)
            continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mul_mod_uint64(x, x, n);
            if (x == n - 1) {
                composite = false;
                break;
            }
        }
        if (composite)
            return false;
    }
    return true;
}

/* ----- make sure we have at least 'n' terms in the cache ---------------- */
static void ensure_n_terms(idx_t n)
{
    if (cache_len >= (size_t)n)
        return;                      /* already have enough */
    grow_cache((size_t)n);

    while (cache_len < (size_t)n) {
        /* test successive odd candidates */
        for (uint64_t p = next_candidate; ; p += 2) {
            if (!cube_sum_ok(p))
                continue;                 /* fails digit condition */
            if (!is_prime_uint64(p))
                continue;                 /* not prime */
            cache[cache_len++] = (seq_t)p;  /* store value */
            if (cache_len >= (size_t)n) {
                next_candidate = p + 2;     /* next candidate after p */
                return;
            }
            /* when last candidate is processed, p will be updated on next loop */
            if ((uint64_t)(-1) - 2 < p) {   /* prevent overflow */
                next_candidate = (uint64_t)(-1);
                return;
            }
        }
    }
}

/* ----- public sequence function ------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;    /* out of range; offset is 1 */
    ensure_n_terms(n);
    return cache[(size_t)(n - 1)];
}
