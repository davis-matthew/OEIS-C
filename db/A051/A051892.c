/*
 * A051892  Values of e, the lesser key or generating number for Pythagorean
 *          triangles in which S (the odd short leg) and U (the hypotenuse)
 *          are twin primes.
 * https://oeis.org/A051892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A051892: Values of e, the lesser key for primitive
 * Pythagorean triples (u = v+1, v) where both 2v+1 and 2v²+2v+1
 * are primes belonging to twin‑prime pairs.
 * Computed by scanning successive v.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- Modular arithmetic for 64‑bit numbers ---------- */
static uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1)
            result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* ---------- Deterministic Miller–Rabin for 64‑bit ---------- */
static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* small prime check */
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,
                                            19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint64_t p = small_primes[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* Miller-Rabin with bases sufficient for n < 3,474,749,660,383
       (our U values never exceed about 5e12). */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t witnesses[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL};
    for (size_t i = 0; i < sizeof(witnesses)/sizeof(witnesses[0]); ++i) {
        uint64_t a = witnesses[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- Check if a prime has a twin prime partner ---------- */
static bool has_twin_prime(uint64_t p) {
    if (p <= 2) return false;
    if (p > 2 && is_prime_uint64(p - 2)) return true;
    return is_prime_uint64(p + 2);
}

/* ---------- Global cache for sequence values ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_size = 0;
static size_t seq_cap  = 0;
static idx_t  checked_v = 0;   /* largest v examined so far */

/* ---------- Ensure we have at least n terms ready ---------- */
static void ensure_terms(idx_t n) {
    while (seq_size < (size_t)n) {
        ++checked_v;                           /* next v to test */
        uint64_t v = checked_v;
        uint64_t S = 2ULL * v + 1ULL;           /* 2v + 1 */
        unsigned __int128 U128 = (unsigned __int128)2ULL * v * v
                               + (unsigned __int128)2ULL * v + 1ULL;
        uint64_t U = (uint64_t)U128;           /* 2v² + 2v + 1 */

        if (is_prime_uint64(S) && has_twin_prime(S) &&
            is_prime_uint64(U) && has_twin_prime(U)) {

            if (seq_size == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap * 2 : 4;
                seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();               /* allocation failed */
                seq_cache = tmp; seq_cap = new_cap;
            }
            seq_cache[seq_size++] = (seq_t)v;     /* e = v */
        }
    }
}

/* ---------- The sequence function ---------- */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* offset is 1; 0 is unused */
    ensure_terms(n);
    return seq_cache[n-1];         /* array is 0‑based */
}
