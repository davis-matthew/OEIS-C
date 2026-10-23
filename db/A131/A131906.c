/*
 * A131906  Smallest positive integer k with the same sum of squares of
 *          divisors as the n-th integer for which such a k exists.
 * https://oeis.org/A131906
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 39, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A131906
   Sequence: a(n) = smallest positive integer k with the same sum of
   squares of divisors as the n-th integer that has such a counterpart.
   Computation: iterate over integers m ≥ 2, computing σ₂(m) (sum of
   squares of divisors) using a smallest‑prime‑factor sieve.  For each
   σ₂ value we keep the first m that produced it.  Whenever a second
   (or later) m is found with an already‑seen σ₂, we store the initial
   m as the next member of the sequence.  All state is kept in static
   tables so that calls can be in any order.  The algorithm is
   O(N) plus a small cost for the hashing of σ₂ values. */

#include <stdint.h>
#include <stdlib.h>

/* ------------------------------------------------------------------ */
/*  Sieve for smallest prime factors (spf).  m is assumed to fit in
    uint64_t.  The table holds 0 for m=0 and 1.                               */
static uint64_t *spf = NULL;
static uint64_t spf_limit = 0;

static void extend_sieve(uint64_t new_limit) {
    uint64_t *ns = (uint64_t *)malloc((new_limit + 1) * sizeof(uint64_t));
    for (uint64_t i = 0; i <= new_limit; ++i) ns[i] = i;
    for (uint64_t i = 2; i * i <= new_limit; ++i) {
        if (ns[i] == i) {
            for (uint64_t j = i * i; j <= new_limit; j += i) {
                if (ns[j] == j) ns[j] = i;
            }
        }
    }
    free(spf);
    spf = ns;
    spf_limit = new_limit;
}

/* ------------------------------------------------------------------ */
/*  Hash table for mapping σ₂ value → smallest m that produced it.
    Empty slots use key 0 (σ₂ values are always > 0).                */
#define HASH_BITS 20
#define HASH_SIZE (1U << HASH_BITS)
#define HASH_MASK (HASH_SIZE - 1)

static seq_t *hash_keys = NULL;   /* keys: σ₂ */
static seq_t *hash_vals = NULL;   /* values: smallest m */

static inline size_t hash_index(seq_t k) {
    unsigned __int128 h = (unsigned __int128)k;
    h ^= h >> 64;
    return (size_t)h & HASH_MASK;
}

static void hash_insert(seq_t key, seq_t val) {
    size_t idx = hash_index(key);
    while (hash_keys[idx] != 0) {
        idx = (idx + 1) & HASH_MASK;
    }
    hash_keys[idx] = key;
    hash_vals[idx] = val;
}

static seq_t hash_lookup(seq_t key) {
    size_t idx = hash_index(key);
    while (hash_keys[idx] != 0) {
        if (hash_keys[idx] == key) return hash_vals[idx];
        idx = (idx + 1) & HASH_MASK;
    }
    return 0;          /* not found */
}

/* ------------------------------------------------------------------ */
/*  Compute σ₂(m) = Σ_{d|m} d²  using the precomputed spf table.      */
static seq_t sigma2_of(uint64_t m) {
    seq_t res = (seq_t)1;
    while (m > 1) {
        uint64_t p = spf[m];
        unsigned exp = 0;
        while (m % p == 0) {
            m /= p;
            ++exp;
        }
        seq_t p2 = (seq_t)p * (seq_t)p;
        seq_t power = (seq_t)1;
        for (unsigned i = 0; i < exp + 1; ++i) power *= p2;      /* p2^(e+1) */
        seq_t num = power - (seq_t)1;
        seq_t denom = p2 - (seq_t)1;
        seq_t term = num / denom;
        res *= term;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/*  Cache of sequence values a(n).                                   */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_size = 0;

/*  Flexible push onto cache.                                        */
static void cache_push(seq_t val) {
    if (cache_size == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 8 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* ------------------------------------------------------------------ */
/*  Main function required by the specification.                      */
seq_t a(idx_t n) {
    static int initialized = 0;
    static uint64_t next_m = 2;

    if (!initialized) {
        /* initial sieve limit: 5 million is sufficient for n≈10 000   */
        uint64_t init_lim = 5000000ULL;
        extend_sieve(init_lim);

        hash_keys = (seq_t *)calloc(HASH_SIZE, sizeof(seq_t));
        hash_vals = (seq_t *)calloc(HASH_SIZE, sizeof(seq_t));

        cache = NULL;
        cache_cap = 0;
        cache_size = 0;

        initialized = 1;
    }

    /* Grow sieve if needed */
    if (next_m > spf_limit) {
        extend_sieve(spf_limit * 2);
    }

    /* Generate terms until we have at least n of them */
    while (cache_size < (size_t)n) {
        seq_t sum = sigma2_of(next_m);
        seq_t existing = hash_lookup(sum);
        if (existing != 0) {
            cache_push(existing);
        } else {
            hash_insert(sum, (seq_t)next_m);
        }
        ++next_m;
    }

    return cache[(size_t)n - 1];
}
