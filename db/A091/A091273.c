/*
 * A091273  Indices of primes of the form k^2 - 11.
 * https://oeis.org/A091273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = π(p_n) where p_n = kth prime of the form k^2‑11 (starting
 *            from k=4)
 * Range:     u8 n <= 8, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   mul_mod64, pow_mod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function generates primes of the quadratic form, keeps a fast sieve for small values,
   and counts prime indices with a binary search in the pre‑computed prime list. */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mul_mod64.h"
#include "../utils/pow_mod64.h"

typedef uint64_t idx_t;    /* index type as required by the build */
typedef seq_t seq_t;       /* seq_t is defined by the build (uint8_t … __int128) */

#define INITIAL_SIEVE_LIMIT 20000000ULL /* 20 million is enough for n≈10 000 */

/* ---------- Miller–Rabin primality test for 64‑bit integers ---------- */

static int is_prime64(uint64_t n) {
    if (n < 2) return 0;
    static const uint64_t small[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        if (n % small[i] == 0) return n == small[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }
    static const uint64_t bases[] = {2,325,9375,28178,450775,9780504,1795265022};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n-1) continue;
        int comp = 1;
        for (uint64_t r = 1; r < s; ++r) {
            x = mul_mod64(x, x, n);
            if (x == n-1) { comp = 0; break; }
        }
        if (comp) return 0;
    }
    return 1;
}

/* ---------- Sieve for fast primality and prime list ---------- */
static uint8_t *sieve = NULL;      /* 0/1 array */
static idx_t *prime_list = NULL;   /* sorted list of primes */
static size_t prime_count = 0;
static idx_t sieve_limit = 0;

static void init_sieve(void) {
    if (sieve_limit) return; /* already initialized */
    sieve_limit = INITIAL_SIEVE_LIMIT;
    sieve = (uint8_t *)malloc(sieve_limit + 1);
    if (!sieve) exit(1); /* fatal if allocation fails */
    memset(sieve, 1, sieve_limit + 1);
    sieve[0] = sieve[1] = 0;
    for (idx_t i = 2; i * i <= sieve_limit; ++i) {
        if (sieve[i]) {
            for (idx_t j = i * i; j <= sieve_limit; j += i) sieve[j] = 0;
        }
    }
    /* count primes first */
    prime_count = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i) if (sieve[i]) ++prime_count;
    prime_list = (idx_t *)malloc(prime_count * sizeof(idx_t));
    if (!prime_list) exit(1);
    size_t pos = 0;
    for (idx_t i = 2; i <= sieve_limit; ++i) if (sieve[i]) prime_list[pos++] = i;
}

/* ---------- Binary search: upper bound in prime_list ---------- */
static idx_t prime_index(idx_t p) {
    /* returns count of primes <= p */
    size_t lo = 0, hi = prime_count;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (prime_list[mid] <= p) lo = mid + 1;
        else hi = mid;
    }
    return (idx_t)lo;
}

/* ---------- Sequence storage ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_cache_len = 0;
static size_t seq_cache_cap = 0;
static idx_t last_k = 4; /* start testing from k=4 */

static void seq_cache_push(seq_t val) {
    if (seq_cache_len == seq_cache_cap) {
        size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 16;
        seq_t *new_mem = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!new_mem) exit(1);
        seq_cache = new_mem;
        seq_cache_cap = new_cap;
    }
    seq_cache[seq_cache_len++] = val;
}

/* ---------- Main function ---------- */
seq_t a(idx_t n) {
    init_sieve();   /* ensure primes available */
    if (n == 0) return 0; /* undefined for n<1, but never called */

    while (seq_cache_len < n) {
        uint64_t k = last_k++;
        uint64_t val = k * k - 11ULL; /* candidate prime */
        int is_prime;
        if (val <= sieve_limit) {
            is_prime = sieve[val];
        } else {
            is_prime = is_prime64(val);
        }
        if (is_prime) {
            idx_t idx = prime_index((idx_t)val);   /* π(val) */
            seq_cache_push((seq_t)idx);
        }
    }
    return seq_cache[n - 1];
}
