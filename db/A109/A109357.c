/*
 * A109357  Numbers k such that the string 112354k is prime.
 * https://oeis.org/A109357
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 2848, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A109357:  k such that 112354k is prime (1‑based).  */
/* The function returns the nth such k.  It is computed by brute‑force
   search with an on‑the‑fly cache of found values.  The algorithm
   stays well below the time limit for n up to about 10000. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mul_mod.h"

static const uint64_t BASE = 112354ULL;

/* ---------- deterministic Miller–Rabin for 64‑bit integers ---------- */

static uint64_t pow_mod(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t res = 1;
    while (e) {
        if (e & 1U) res = mul_mod(res, a, mod);
        a = mul_mod(a, a, mod);
        e >>= 1U;
    }
    return res;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2U) return false;
    static const uint64_t primes[] = {
        2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL,41ULL,43ULL,47ULL};
    for (size_t i = 0; i < sizeof(primes)/sizeof(primes[0]); ++i) {
        if (n % primes[i] == 0) return n == primes[i];
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1ULL; ++s; }
    static const uint64_t bases[] = {
        2ULL,325ULL,9375ULL,28178ULL,450775ULL,9780504ULL,1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0ULL) continue;
        uint64_t x = pow_mod(a, d, n);
        if (x == 1ULL || x == n - 1ULL) continue;
        bool composite = true;
        for (uint64_t r = 1ULL; r < s; ++r) {
            x = mul_mod(x, x, n);
            if (x == n - 1ULL) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- dynamic array for found k-values ---------- */

static uint64_t *seq_arr = NULL;
static idx_t seq_cap = 0;
static idx_t seq_len = 0;

static void ensure_cap(idx_t min_cap)
{
    if (seq_cap >= min_cap) return;
    idx_t new_cap = seq_cap ? seq_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_arr = (uint64_t*)realloc(seq_arr, new_cap * sizeof(uint64_t));
    seq_cap = new_cap;
}

static void push_val(uint64_t v)
{
    ensure_cap(seq_len + 1);
    seq_arr[seq_len++] = v;
}

/* ---------- search state ---------- */

static uint64_t next_multiplier = 10ULL; /* 10^digits(k) */
static uint64_t cur_k = 1ULL;

/* generate terms until we have at least n elements */
static void generate_upto(idx_t n)
{
    while (seq_len < n) {
        /* update power of ten when we move to a new digit length */
        if (cur_k == next_multiplier) next_multiplier *= 10ULL;

        unsigned __int128 cand = (unsigned __int128)BASE * next_multiplier + cur_k;
        if (cand <= UINT64_MAX) {
            if (is_prime_uint64((uint64_t)cand))
                push_val(cur_k);
            ++cur_k;
        } else {
            /* The number is too large for 64‑bit.  In the range of interest
               (n ≤ 10000) this branch is never reached, so we simply
               skip the remaining values. */
            ++cur_k;
        }
    }
}

/* ---------- public function ---------- */

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* defined for n ≥ 1 only */
    generate_upto(n);
    return (seq_t)seq_arr[n-1];
}
