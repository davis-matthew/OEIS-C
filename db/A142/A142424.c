/*
 * A142424  Primes congruent to 12 mod 49.
 * https://oeis.org/A142424
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 157, u32 n <= 158, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements A142424: primes congruent to 12 (mod 49), found by incremental
   search with a deterministic primality test. */

#include <stdlib.h>
#include <stdbool.h>

/* ---------- Helper: modular exponentiation for arbitrary seq_t ---------- */
static seq_t mod_pow_seq(seq_t base, seq_t exp, seq_t mod)
{
    seq_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* ---------- Helper: deterministic Miller–Rabin primality test ---------- */
static bool is_prime_big(seq_t n)
{
    /* n < 2 */
    if (n < 2)
        return false;

    /* small primes up to 300 */
    static const uint64_t small_primes[] = {
        2, 3, 5, 7, 11, 13, 17, 19, 23, 29,
        31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
        73, 79, 83, 89, 97, 101, 103, 107, 109, 113,
        127, 131, 137, 139, 149, 151, 157, 163, 167, 173,
        179, 181, 191, 193, 197, 199, 211, 223, 227, 229,
        233, 239, 241, 251, 257, 263, 269, 271, 277, 281,
        283, 293, 307, 311, 313, 317, 331, 337, 347, 349,
        353, 359, 367, 373, 379, 383, 389, 397, 401, 409,
        419, 421, 431, 433, 439, 443, 449, 457, 461, 463,
        467, 479, 487, 491, 499, 503, 509, 521, 523, 541,
        547, 557, 563, 569, 571, 577
    };
    static const size_t small_primes_len = sizeof(small_primes) / sizeof(small_primes[0]);

    for (size_t i = 0; i < small_primes_len; ++i) {
        uint64_t p = small_primes[i];
        if (n == (seq_t)p)
            return true;
        if (n % (seq_t)p == 0)
            return false;
    }

    /* write n-1 as d * 2^r with d odd */
    seq_t d = n - 1;
    int r = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++r;
    }

    /* deterministic bases for 64‑bit integers, safe for much larger values
       (probabilistic for very large integers, but sufficient for test data). */
    static const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    static const size_t bases_len = sizeof(bases) / sizeof(bases[0]);

    for (size_t i = 0; i < bases_len; ++i) {
        seq_t a = (seq_t)bases[i] % n;
        if (a == 0)
            continue;

        seq_t x = mod_pow_seq(a, d, n);
        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;
        for (int j = 1; j < r; ++j) {
            x = (x * x) % n;
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

/* ---------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    /* cache of found primes congruent to 12 (mod 49) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_k = 0;        /* last processed k, where p = 49*k + 12 */

    if (n == 0)                /* undefined, return 0 for robustness */
        return (seq_t)0;

    /* initial allocation if needed */
    if (cache_len == 0) {
        cache_cap = 64;                    /* start with moderate size */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        idx_t k = last_k + 1;
        seq_t candidate = (seq_t)49 * (seq_t)k + (seq_t)12;

        if (is_prime_big(candidate)) {
            if (cache_len >= cache_cap) {
                cache_cap = cache_cap * 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = candidate;
        }
        last_k = k;
    }

    return cache[n - 1];
}
