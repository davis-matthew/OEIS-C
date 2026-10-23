/*
 * A139843  Primes of the form 6x^2 + 17y^2.
 * https://oeis.org/A139843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 831, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _allowed_residue
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: the nth prime of the form 6x²+17y².
 * The primes are the ones congruent to one of the 18 residues
 *  {17, 23, 41, 65, 71, 95, 113, 143, 167, 209, 215,
 *   233, 311, 329, 335, 377, 401} modulo 408.
 *
 * This implementation generates primes on demand, keeping the already
 * found primes in a cache and using a deterministic Miller‑Rabin test
 * for 64‑bit values.  The sequence is memoised, so calls in any order
 * are supported.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/_allowed_residue.h"

/* ---------- local helpers ------------------------------------- */

// Deterministic Miller‑Rabin for u64.
static bool _is_prime_u64(uint64_t n)
{
    if (n < 2) return false;
    static const uint64_t small_primes[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL,19ULL,23ULL,29ULL,31ULL,37ULL};
    for (size_t i = 0; i < sizeof(small_primes)/sizeof(small_primes[0]); ++i) {
        if (n == small_primes[i]) return true;
        if (n % small_primes[i] == 0) return false;
    }
    uint64_t d = n - 1, s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    // bases sufficient for deterministic test for 64‑bit integers
    static const uint64_t bases[] = {2ULL, 325ULL, 9375ULL, 28178ULL, 450775ULL, 9780504ULL, 1795265022ULL};
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i) {
        uint64_t a = bases[i] % n;
        if (a == 0) continue;
        __uint128_t x = 1, p = a, e = d;
        // fast exponentiation: x = a^d mod n
        __uint128_t mod = n;
        __uint128_t res = 1;
        __uint128_t base = a % n;
        while (e) {
            if (e & 1) res = (res * base) % mod;
            base = (base * base) % mod;
            e >>= 1;
        }
        x = res;
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (uint64_t r = 1; r < s; ++r) {
            x = (x * x) % mod;
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

/* ---------- cache structures ------------------------------------- */

static seq_t *seq_cache = NULL;     /* a(1) is stored at seq_cache[0]    */
static size_t seq_cap  = 0;         /* allocated capacity of seq_cache    */
static size_t seq_len  = 0;         /* number of terms currently stored   */

static uint64_t *prime_cache = NULL;
static size_t prime_cap   = 0;
static size_t prime_len   = 0;
static uint64_t last_checked_prime = 2;  // start after the first prime

/* ---------- dynamic array helpers --------------------------------- */

static void _grow_seq_cache(void)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (seq_cache) { memcpy(new_cache, seq_cache, seq_len * sizeof(seq_t)); free(seq_cache); }
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

static void _grow_prime_cache(void)
{
    size_t new_cap = prime_cap ? prime_cap * 2 : 16;
    uint64_t *new_list = (uint64_t *)malloc(new_cap * sizeof(uint64_t));
    if (prime_cache) { memcpy(new_list, prime_cache, prime_len * sizeof(uint64_t)); free(prime_cache); }
    prime_cache = new_list;
    prime_cap   = new_cap;
}

/* ---------- public function --------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety: undefined for n==0 */

    /* expand the cache until we have at least n terms */
    while (seq_len < n) {
        /* generate next odd candidate */
        uint64_t cand = last_checked_prime;
        if (cand <= 2) cand = 3;
        /* skip even numbers */
        if (cand % 2 == 0) cand++;
        last_checked_prime = cand + 2;

        if (_is_prime_u64(cand) && _allowed_residue(cand)) {
            if (seq_len == seq_cap) _grow_seq_cache();
            seq_cache[seq_len++] = (seq_t)cand;
        }
    }
    return seq_cache[n - 1];
}
