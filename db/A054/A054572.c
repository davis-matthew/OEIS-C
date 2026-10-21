/*
 * A054572  Larger members of g-reduced amicable pairs a < b such that
 *          sigma(a) = sigma(b) = a + b + gcd(a,b).
 * https://oeis.org/A054572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   gcd, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A054572: larger members of g‑reduced amicable pairs a < b
 *  that satisfy σ(a)=σ(b)=a+b+gcd(a,b).  The function returns the
 *  n‑th term of this sequence (offset 1).  The algorithm scans
 *  successive integers b, computes σ(b), and for each b tests all
 *  earlier a having the same σ(b).  Matches are appended to the
 *  result list.  The search is incremental and memoised so that
 *  calls to a(n) in any order work correctly. */
    
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/sigma_of.h"

typedef struct {
    seq_t      sigma;      /* key */
    uint64_t  *nums;       /* list of a's sharing this sigma */
    size_t     len;
    size_t     cap;
} bucket_t;

/* ---------- global state ---------- */
static seq_t    *terms   = NULL;   /* found b's (sequence values) */
static size_t   terms_len = 0;     /* current number of terms stored */
static size_t   terms_cap = 0;     /* capacity of terms array */

static bucket_t *hash     = NULL;  /* hash table of buckets */
static size_t   hash_size = 1 << 20; /* 1 M buckets, power of two */
static size_t   hash_used = 0;      /* number of occupied buckets */

static uint64_t current_b = 0;      /* last processed number */

/* ---------- utilities ---------- */
static void grow_terms(void) {
    size_t new_cap = (terms_cap == 0) ? 1024 : terms_cap * 2;
    seq_t *new_ptr = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();
    terms = new_ptr;
    terms_cap = new_cap;
}

static void bucket_grow(bucket_t *b) {
    size_t new_cap = (b->cap == 0) ? 4 : b->cap * 2;
    uint64_t *new_ptr = (uint64_t *)realloc(b->nums, new_cap * sizeof(uint64_t));
    if (!new_ptr) abort();
    b->nums = new_ptr;
    b->cap  = new_cap;
}

static void bucket_append(bucket_t *b, uint64_t val) {
    if (b->len == b->cap) bucket_grow(b);
    b->nums[b->len++] = val;
}

/* ---------- hash helpers ---------- */
static void init_hash(void) {
    if (hash) return;
    hash = (bucket_t *)calloc(hash_size, sizeof(bucket_t));
    if (!hash) abort();
}

static inline size_t seq_hash(seq_t x) {
    /* simple hash: truncation for 64-bit buckets */
    return (size_t)((uint64_t)x) & (hash_size - 1);
}

static bucket_t *bucket_lookup(seq_t sigma, bool create) {
    init_hash();
    size_t idx = seq_hash(sigma);
    while (1) {
        bucket_t *b = &hash[idx];
        if (b->len == 0) {
            if (!create) return NULL;
            /* create new bucket */
            b->sigma = sigma;
            b->len   = 0;
            b->cap   = 0;
            b->nums  = NULL;
            hash_used++;
            return b;
        }
        if (b->sigma == sigma) return b;
        idx = (idx + 1) & (hash_size - 1);
    }
}

static void rehash(void) {
    size_t new_size = hash_size * 2;
    bucket_t *new_hash = (bucket_t *)calloc(new_size, sizeof(bucket_t));
    if (!new_hash) abort();

    for (size_t i = 0; i < hash_size; ++i) {
        bucket_t *b = &hash[i];
        if (b->len == 0) continue;
        size_t idx = ((size_t)b->sigma) & (new_size - 1);
        while (new_hash[idx].len != 0) idx = (idx + 1) & (new_size - 1);
        new_hash[idx] = *b;          /* copy bucket data */
    }
    free(hash);
    hash = new_hash;
    hash_size = new_size;
    /* hash_used unchanged */
}

/* ---------- core routine ---------- */
static void ensure_terms(idx_t n_max) {
    if (terms_len >= (size_t)n_max) return;

    while (terms_len < (size_t)n_max) {
        current_b++;                                     /* next integer to test */
        seq_t sigma_b = sigma_of((idx_t)current_b);       /* σ(b) */

        /* bucket for this σ value */
        bucket_t *bkt = bucket_lookup(sigma_b, true);

        /* test against all earlier a with same σ */
        for (size_t i = 0; i < bkt->len; ++i) {
            uint64_t a_val = bkt->nums[i];
            seq_t sum_ab   = (seq_t)a_val + (seq_t)current_b;
            if (sigma_b <= sum_ab) continue;               /* would make g <= 0 */
            seq_t g = sigma_b - sum_ab;                    /* candidate gcd */

            seq_t g2 = gcd((seq_t)a_val, (seq_t)current_b);
            if (g2 == g) {
                /* record current_b as a term (allow duplicates) */
                if (terms_len == terms_cap) grow_terms();
                terms[terms_len++] = (seq_t)current_b;
            }
        }

        /* add current_b to its bucket for future matches */
        bucket_append(bkt, current_b);

        /* keep load factor below ~0.7 */
        if (hash_used > (hash_size * 7) / 10) rehash();
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;           /* not used, but safe */
    ensure_terms(n);
    return terms[(size_t)(n - 1)];
}
