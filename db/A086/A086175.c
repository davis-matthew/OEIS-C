/*
 * A086175  Numbers n such that n*prime(n)-2 is prime.
 * https://oeis.org/A086175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that n*prime(n)-2 is prime.
   a(n) returns the n-th integer n that satisfies this property. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"

/* ---------- 64‑bit deterministic Miller–Rabin primality test ---------- */

static uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1U) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool isPrime64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i=0;i<sizeof(small)/sizeof(small[0]);++i) {
        if (n % small[i] == 0ULL) return n == small[i];
    }
    uint64_t d = n-1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i=0;i<sizeof(bases)/sizeof(bases[0]);++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) return true;
        uint64_t x = pow_mod64(a, d, n);
        if (x==1 || x==n-1) continue;
        bool cont = false;
        for (uint64_t r=1;r<s;++r) {
            x = mul_mod64(x, x, n);
            if (x==n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- Prime list generation ------------------------------------ */
#define INITIAL_PRIME_CAP 65536U
static uint32_t *prime_list    = NULL;
static size_t   prime_count   = 0;
static size_t   prime_cap     = 0;
static uint32_t prime_candidate = 2UL;  /* next number to test for primality */

static void init_prime_list(void)
{
    if (prime_list != NULL) return;
    prime_cap = INITIAL_PRIME_CAP;
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    if (!prime_list) exit(1);
    prime_count = 0;
    /* add the first prime 2 */
    prime_list[prime_count++] = 2U;
    prime_candidate = 3U;
}

static void grow_prime_list(void)
{
    prime_cap = prime_cap ? prime_cap * 2 : INITIAL_PRIME_CAP;
    uint32_t *tmp = (uint32_t *)realloc(prime_list, prime_cap * sizeof(uint32_t));
    if (!tmp) exit(1);
    prime_list = tmp;
}

static void ensure_prime_index(size_t idx)   /* idx is 1‑based */
{
    init_prime_list();
    while (prime_count < idx) {
        while (!isPrime64(prime_candidate)) { prime_candidate += 2; }
        if (prime_count == prime_cap) grow_prime_list();
        prime_list[prime_count++] = (uint32_t)prime_candidate;
        prime_candidate += 2;
    }
}

/* ---------- Cache for sequence terms --------------------------------- */
static seq_t *term_cache  = NULL;
static size_t cache_len   = 0;
static size_t cache_cap   = 0;
static uint32_t m_processed = 0;   /* how many m's have already been examined */

static void grow_term_cache(void)
{
    cache_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    term_cache = tmp;
}

/* ---------- Main sequence function ------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, safe fallback */

    /* expand cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        ++m_processed;                 /* current candidate m (1-based) */
        ensure_prime_index((size_t)m_processed);
        uint32_t p = prime_list[m_processed - 1];

        uint64_t prod = (uint64_t)m_processed * (uint64_t)p - 2ULL;
        if (isPrime64(prod)) {
            if (cache_len == cache_cap) grow_term_cache();
            term_cache[cache_len++] = (seq_t)m_processed;
        }
    }
    return term_cache[(size_t)n - 1];
}
