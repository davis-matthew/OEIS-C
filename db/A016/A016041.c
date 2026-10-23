/*
 * A016041  Primes that are palindromic in base 2 (but written here in base
 *          10).
 * https://oeis.org/A016041
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 6756, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A016041 : primes that are palindromic in base 2 (given in base 10). */
/* The function generates binary palindromes in increasing order and tests
 * each one for primality.  For 64‑bit builds a deterministic Miller–Rabin
 * test is used; otherwise the generic is_prime_big helper is called. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/is_prime_big.h"

/* Reverse the lowest 'len' bits of x (works for any seq_t type). */
static seq_t reverse_bits(seq_t x, int len) {
    seq_t rev = 0;
    for (int i = 0; i < len; ++i) {
        rev = (rev << 1) | (x & 1);
        x >>= 1;
    }
    return rev;
}

/* Construct the binary palindrome of given length 'len' using the most
 * significant half (including the leading 1) provided in 'half'. */
static seq_t make_pal(idx_t len, idx_t half) {
    seq_t half_t = (seq_t)half;
    int half_len = (len + 1) / 2;

    if ((len & 1) == 0) {                /* even length */
        seq_t rev = reverse_bits(half_t, half_len);
        return (half_t << half_len) | rev;
    } else {                              /* odd length */
        seq_t rev = reverse_bits(half_t >> 1, half_len - 1);
        return (half_t << (half_len - 1)) | rev;
    }
}

/* -----------  deterministic Miller–Rabin for 64‑bit numbers  ----------- */
static inline uint64_t mod_mul_uint64(uint64_t a, uint64_t b, uint64_t mod) {
#if defined(__SIZEOF_INT128__)
    return (unsigned __int128)a * b % mod;
#else
    /* Fallback for systems without __int128 (unlikely in tests). */
    uint64_t res = 0;
    while (b) {
        if (b & 1) res = (res + a) % mod;
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
#endif
}

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mod_mul_uint64(result, base, mod);
        base = mod_mul_uint64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    /* Trial division by small primes */
    static const uint32_t small_primes[] = {
        2,3,5,7,11,13,17,19,23,29,31,37
    };
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        uint32_t p = small_primes[i];
        if (n % p == 0) return n == p;
    }

    /* Write n-1 as d*2^s */
    uint64_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic bases for 64‑bit */
    static const uint64_t bases[] = {
        2ULL, 325ULL, 9375ULL, 28178ULL,
        450775ULL, 9780504ULL, 1795265022ULL
    };
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = mod_pow_uint64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (unsigned r = 1; r < s; ++r) {
            x = mod_mul_uint64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

static bool is_prime_seq(seq_t n) {
    if (sizeof(seq_t) <= 8) {
        return is_prime_u64((uint64_t)n);
    } else {
        return is_prime_big(n);
    }
}

/* -----------  cache of generated palindromic primes  ----------- */
static seq_t *pal_prime_cache = NULL;
static size_t cache_size   = 0;
static size_t cache_cap    = 0;

/* -----------  state for generating binary palindromes  ----------- */
static idx_t gen_len   = 1;   /* current binary length (>=1) */
static idx_t gen_half  = 0;   /* current half (0 means start of length) */

/* Produce the next binary palindrome in increasing order. */
static seq_t next_palindrome(void) {
    while (1) {
        int half_len   = (gen_len + 1) / 2;
        idx_t half_start = (idx_t)1 << (half_len - 1);
        idx_t half_end   = ((idx_t)1 << half_len) - 1;

        if (gen_half == 0)
            gen_half = half_start;

        if (gen_half > half_end) {   /* finished this length */
            ++gen_len;
            gen_half = 0;
            continue;
        }

        seq_t pal = make_pal(gen_len, gen_half);
        ++gen_half;                 /* advance for next call */
        return pal;
    }
}

/* Ensure the cache contains at least 'need' palindromic primes. */
static void ensure_cache(idx_t need) {
    if ((size_t)need <= cache_size)
        return;

    if (pal_prime_cache == NULL) {
        cache_cap = 64;
        pal_prime_cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!pal_prime_cache)
            exit(EXIT_FAILURE);
    }

    while (cache_size < (size_t)need) {
        seq_t candidate = next_palindrome();
        if (is_prime_seq(candidate)) {
            if (cache_size == cache_cap) {
                cache_cap <<= 1;   /* double capacity */
                seq_t *tmp = (seq_t *)realloc(pal_prime_cache, cache_cap * sizeof(seq_t));
                if (!tmp) {
                    free(pal_prime_cache);
                    exit(EXIT_FAILURE);
                }
                pal_prime_cache = tmp;
            }
            pal_prime_cache[cache_size++] = candidate;
        }
    }
}

/* Public sequence function. */
seq_t a(idx_t n) {
    if (n == 0)            /* undefined for offset 0 */
        return 0;
    ensure_cache(n);
    return pal_prime_cache[n - 1];
}
