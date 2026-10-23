/*
 * A028879  Numbers k such that k^2 - 6 is prime.
 * https://oeis.org/A028879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th integer k such that k^2 - 6 is prime.
 *  The function searches for k incrementally, caching results for
 *  efficiency and supporting out-of-order requests. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---- Miller–Rabin primality test for 64‑bit unsigned integers ---- */
static inline uint64_t mulmod(uint64_t a, uint64_t b, uint64_t mod) {
    return (uint64_t)((__int128)a * b % mod);
}

static inline uint64_t powmod(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    while (exp) {
        if (exp & 1) result = mulmod(result, base, mod);
        base = mulmod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }

    /* write n-1 as d*2^s */
    uint64_t d = n - 1,
             s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }

    /* Deterministic set of bases for 64‑bit integers */
    const uint64_t bases[] = {2,3,5,7,11,13,17};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = powmod(a, d, n);
        if (x == 1 || x == n-1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod(x, x, n);
            if (x == n-1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---- Sequence memoization and computation ---- */
static seq_t *seq_cache = NULL;
static size_t seq_len   = 0;
static size_t seq_cap   = 0;
static idx_t    next_k  = 1;      /* next integer k to test */

static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size) {
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : *cap * 2;
    if (new_cap < needed) new_cap = needed;
    *ptr = realloc(*ptr, new_cap * elem_size);
    *cap = new_cap;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* undefined for offset 1, but safe */

    /* extend cache up to index n */
    while (seq_len < (size_t)n) {
        uint64_t pk = (uint64_t)next_k * (uint64_t)next_k - 6;
        if (is_prime_uint64(pk)) {
            ensure_capacity(seq_len + 1, &seq_cap, (void**)&seq_cache, sizeof(seq_t));
            seq_cache[seq_len++] = next_k;
        }
        ++next_k;
    }
    return seq_cache[n-1];
}
