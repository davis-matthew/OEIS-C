/*
 * A145688  Primitive prime factors of the sequence 2k^2 + 1 in the order that
 *          they are found.
 * https://oeis.org/A145688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A145688: Primitive prime factors of 2k²+1 in the order of appearance.
   The sequence a(n) is the n‑th distinct prime that first divides some
   2k²+1.  This implementation keeps a cache of the primes found so far
   and examines successive integers 2k²+1 until the cache has at least
   n elements.  Factorisation is done by trial division using a
   pre‑computed list of primes, regenerated as needed. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/_floor_isqrt_u64.h"      /* _floor_isqrt_u64(idx_t) */
#include "../utils/build_primes_upto.h"     /* build_primes_upto(idx_t, uint64_t **, size_t *) */

static seq_t *seq_cache = NULL;  /* cached distinct primes */
static size_t seq_cap  = 0;      /* allocated size of seq_cache */
static size_t seq_len  = 0;      /* number of primes stored */

static idx_t k_pos = 1;         /* next k to process (starts at 1) */

/* list of primes used for trial division */
static uint64_t *prime_list = NULL;
static size_t   prime_cnt   = 0;
static idx_t    prime_limit = 0; /* largest prime generated */

/* small hash table to record which primes have already been seen */
typedef struct {
    uint64_t *keys;   /* 0 == empty slot; primes are positive */
    size_t   cap;     /* power of two */
    size_t   used;
} seen_table_t;

static seen_table_t seen = {0};

static void seen_init(void)
{
    seen.cap  = 32;
    seen.keys = (uint64_t*)calloc(seen.cap, sizeof(uint64_t));
    seen.used = 0;
}

static void seen_expand(void)
{
    size_t new_cap  = seen.cap << 1;
    uint64_t *new_keys = (uint64_t*)calloc(new_cap, sizeof(uint64_t));
    for (size_t i = 0; i < seen.cap; ++i) {
        uint64_t v = seen.keys[i];
        if (v == 0) continue;
        size_t h = v & (new_cap - 1);
        while (new_keys[h] != 0) h = (h + 1) & (new_cap - 1);
        new_keys[h] = v;
    }
    free(seen.keys);
    seen.keys = new_keys;
    seen.cap  = new_cap;
}

static bool seen_contains(uint64_t p)
{
    if (!seen.keys) return false;
    size_t h = p & (seen.cap - 1);
    while (true) {
        uint64_t v = seen.keys[h];
        if (v == 0) return false;
        if (v == p) return true;
        h = (h + 1) & (seen.cap - 1);
    }
}

static void seen_insert(uint64_t p)
{
    if (!seen.keys) seen_init();
    if (seen.used * 2 >= seen.cap) seen_expand();
    size_t h = p & (seen.cap - 1);
    while (true) {
        uint64_t v = seen.keys[h];
        if (v == 0) {
            seen.keys[h] = p;
            ++seen.used;
            return;
        }
        if (v == p) return;
        h = (h + 1) & (seen.cap - 1);
    }
}

/* Build (or rebuild) the prime list up to `limit`. */
static void build_prime_list(idx_t limit)
{
    if (prime_list) {
        free(prime_list);
        prime_list = NULL;
    }
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Ensure that the prime list contains all primes <= `bound`. */
static void ensure_prime_limit(idx_t bound)
{
    if (!prime_list || prime_limit < bound) {
        build_prime_list(bound);
    }
}

/* Factorise `n` and add any new distinct prime factors to the cache. */
static void add_factor_primes(uint64_t n)
{
    if (n <= 1) return;

    uint64_t remaining = n;
    for (size_t i = 0; i < prime_cnt &&
         (uint64_t)prime_list[i] * (uint64_t)prime_list[i] <= remaining; ++i) {
        uint64_t p = prime_list[i];
        if (remaining % p == 0) {
            if (!seen_contains(p)) {
                seen_insert(p);
                /* Grow the cache if necessary. */
                if (seq_len == seq_cap) {
                    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
                    seq_cache = (seq_t*)realloc(seq_cache,
                                                new_cap * sizeof(seq_t));
                    seq_cap   = new_cap;
                }
                seq_cache[seq_len++] = (seq_t)p;
            }
            while (remaining % p == 0) remaining /= p;
        }
    }
    if (remaining > 1) { /* remaining is a prime > sqrt(n) */
        uint64_t p = remaining;
        if (!seen_contains(p)) {
            seen_insert(p);
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap ? seq_cap << 1 : 8;
                seq_cache = (seq_t*)realloc(seq_cache,
                                            new_cap * sizeof(seq_t));
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)p;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined for offset zero */

    /* Generate terms until we have at least n values. */
    while (seq_len < (size_t)n) {
        uint64_t kk = (uint64_t)k_pos;
        uint64_t N  = 2 * kk * kk + 1;

        /* Ensure we have enough primes to factorise N. */
        uint64_t sqrtN = _floor_isqrt_u64(N) + 1;
        ensure_prime_limit(sqrtN);

        add_factor_primes(N);
        ++k_pos;
    }
    return seq_cache[n - 1];
}
