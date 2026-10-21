/*
 * A084996  Numbers which can be written as the product of two distinct primes
 *          and containing only prime decimal digits.
 * https://oeis.org/A084996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mod_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A084996 – Numbers that are the product of two distinct primes and consist only of prime decimal digits (2,3,5,7). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_mul.h"

/* ------------------------------------------------------------------ */
/* ---   Miller‑Rabin primality test for 64‑bit integers              */

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul(result, base, mod);
        base = mod_mul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t smalls[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(smalls)/sizeof(smalls[0]); ++i) {
        if (n == smalls[i]) return true;
        if (n % smalls[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ------------------------------------------------------------------ */
/* ---   Sieve to obtain primes up to a limit                         */
/* ------------------------------------------------------------------ */
#define PRIME_LIMIT 1000000U
static uint64_t primes_list[PRIME_LIMIT];
static size_t prime_cnt = 0;
static bool primes_initialized = false;

static void init_primes(void)
{
    if (primes_initialized) return;
    bool *is_comp = (bool*)calloc(PRIME_LIMIT + 1, 1);
    for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (!is_comp[i]) {
            for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i) is_comp[j] = true;
        }
    }
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) if (!is_comp[i]) {
        primes_list[prime_cnt++] = i;
    }
    free(is_comp);
    primes_initialized = true;
}

/* ------------------------------------------------------------------ */
/* ---   Check if a 64‑bit integer is a product of two distinct primes */
/* ------------------------------------------------------------------ */
static bool is_semiprime_distinct(uint64_t n)
{
    if (n < 6) return false;
    for (size_t i = 0; i < prime_cnt && primes_list[i] * primes_list[i] <= n; ++i) {
        uint64_t p = primes_list[i];
        if (n % p == 0) {
            uint64_t q = n / p;
            if (q == p) return false;          // same prime twice
            return is_prime_u64(q);           // other factor must be prime
        }
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* ---   Recursive generation of numbers with prime decimal digits    */
/* ------------------------------------------------------------------ */
static const int PRIMEDIGITS[4] = {2, 3, 5, 7};

static seq_t *cache   = NULL;
static size_t cached_len = 0;
static idx_t target_n = 0;

static bool rec_gen(int pos, int len, seq_t val)
{
    if (cache == NULL) return false;   /* safety */
    if (pos == len) {
        /* candidate complete; test for semiprime with distinct primes */
        if (is_semiprime_distinct((uint64_t)val)) {
            cache[cached_len++] = val;
            if (cached_len >= target_n) return true; /* stop at target */
        }
        return false;
    }
    for (int d = 0; d < 4; ++d) {
        seq_t newval = val * 10 + PRIMEDIGITS[d];
        if (rec_gen(pos + 1, len, newval)) return true;
    }
    return false;
}

/* ------------------------------------------------------------------ */
/* ---   Build cache up to required index                            */
/* ------------------------------------------------------------------ */
static void build_up_to(idx_t n)
{
    free(cache);
    cache        = (seq_t*)malloc(n * sizeof(seq_t));
    cached_len   = 0;
    target_n     = n;

    init_primes();                /* primes list ready */

    for (int len = 1; cached_len < target_n; ++len) {
        rec_gen(0, len, 0);        /* continues until cached_len == target_n */
    }
}

/* ------------------------------------------------------------------ */
/* ---   Public sequence function                                    */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for offset 1, but harmless */
    if (cached_len >= n) return cache[n - 1];
    build_up_to(n);
    return cache[n - 1];
}
