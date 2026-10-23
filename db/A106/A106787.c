/*
 * A106787  Primes with digit sum = 61.
 * https://oeis.org/A106787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   mulmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes with digit sum = 61 */
/* The function enumerates numbers of increasing length that sum to 61,
   tests each for primality, and caches the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mulmod64.h"

/* Local dynamic array for cached primes */
static seq_t *prime_cache = NULL;
static idx_t prime_capacity = 0;
static idx_t prime_count   = 0;

/* Last length that has been fully processed (starting at 6, so we
   begin with length 7). */
static idx_t last_processed_length = 6;

/* Ensure capacity for at least 'min_cap' elements. */
static void ensure_cache_capacity(idx_t min_cap) {
    if (prime_capacity >= min_cap) return;
    idx_t new_cap = prime_capacity == 0 ? 8 : prime_capacity * 2;
    while (new_cap < min_cap) new_cap *= 2;
    seq_t *new_arr = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
    prime_cache = new_arr;
    prime_capacity = new_cap;
}

/* Append a prime value to the cache. */
static void append_prime(seq_t value) {
    ensure_cache_capacity(prime_count + 1);
    prime_cache[prime_count++] = value;
}

/* ---------- Miller‑Rabin for 64‑bit integers ---------- */

static uint64_t powmod64(uint64_t a, uint64_t e, uint64_t mod) {
    uint64_t r = 1 % mod;
    while (e) {
        if (e & 1) r = mulmod64(r, a, mod);
        a = mulmod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n) {
    if (n < 2) return false;
    /* small primes */
    static const uint64_t small[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small)/sizeof(small[0]); ++i) {
        uint64_t p = small[i];
        if (n == p) return true;
        if (n % p == 0) return false;
    }
    /* write n‑1 = d·2ˢ */
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        ++s;
    }
    /* bases that certify all 64‑bit numbers */
    static const uint64_t bases[] = {2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        uint64_t x = powmod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = mulmod64(x, x, n);
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- Recursive generator for fixed length ---------- */

static void dfs_len(idx_t L, idx_t pos, idx_t rem_sum, seq_t cur) {
    if (pos == L) return; /* should not happen */
    if (pos == L - 1) {
        /* last digit must be the remaining sum and must be odd */
        if (rem_sum <= 9 && rem_sum >= (pos == 0 ? 1 : 0) && (rem_sum % 2) == 1) {
            seq_t val = cur * 10 + rem_sum;
            uint64_t n = (uint64_t)val;          /* safe: numbers stay < 2⁶⁴ */
            if (is_prime_uint64(n)) append_prime(val);
        }
        return;
    }
    int min_d = (pos == 0) ? 1 : 0;
    for (int d = min_d; d <= 9; ++d) {
        if ((idx_t)d > rem_sum) break;
        idx_t remaining = L - 1 - pos;
        if (rem_sum - d > 9 * remaining) continue; /* insufficient digits left */
        dfs_len(L, pos + 1, rem_sum - d, cur * 10 + d);
    }
}

static void generate_len(idx_t L) {
    dfs_len(L, 0, 61, 0);
}

/* ---------- Ensure enough primes are cached ---------- */

static void generate_to(idx_t target) {
    while (prime_count < target) {
        ++last_processed_length;
        generate_len(last_processed_length);
    }
}

/* ---------- Public interface ---------- */

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* undefined for n<1, but safe */
    generate_to(n);
    return prime_cache[n - 1];
}
