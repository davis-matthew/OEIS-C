/*
 * A106697  Numbers k such that 997*k - 101 is prime.
 * https://oeis.org/A106697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A106697: numbers k such that 997*k – 101 is prime.
   Computed by brute‑force search with a deterministic 64‑bit
   Miller‑Rabin primality test. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* --------- primality helpers for uint64_t --------- */
static inline uint64_t mod_mul(uint64_t a, uint64_t b, uint64_t mod) {
    return (unsigned __int128)a * b % mod;
}

static inline uint64_t mod_pow(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    uint64_t b = base % mod;
    while (exp) {
        if (exp & 1) result = mod_mul(result, b, mod);
        b = mod_mul(b, b, mod);
        exp >>= 1;
    }
    return result;
}

/* deterministic Miller‑Rabin for 64‑bit integers */
static inline bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n % p == 0) return n == p;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    /* bases for deterministic test up to 2^64 */
    static const uint64_t bases[] =
        {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};

    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = mod_pow(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (int r = 1; r < s; ++r) {
            x = mod_mul(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}
/* -------------------------------------------- */

static seq_t *k_cache = NULL;
static idx_t k_cache_cap = 0;
static idx_t k_cache_size = 0;
static uint64_t next_k = 1;

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n < 1, but safe */

    /* lazy initialization */
    if (k_cache == NULL) {
        k_cache_cap = 128;
        k_cache = (seq_t *)malloc(k_cache_cap * sizeof(seq_t));
        k_cache_size = 0;
        next_k = 1;
    }

    /* generate terms until we have the nth one */
    while (k_cache_size < n) {
        /* search for the next k such that 997*k - 101 is prime */
        while (true) {
            uint64_t p = 997ULL * next_k - 101ULL;
            if (is_prime_uint64(p)) {
                /* store this k */
                if (k_cache_size >= k_cache_cap) {
                    k_cache_cap <<= 1;               /* double capacity */
                    k_cache = (seq_t *)realloc(k_cache, k_cache_cap * sizeof(seq_t));
                }
                k_cache[k_cache_size++] = (seq_t)next_k;
                next_k++;                            /* advance to the next candidate */
                break;
            }
            next_k++;
        }
    }

    return k_cache[n - 1];
}
