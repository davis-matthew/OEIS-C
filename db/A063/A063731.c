/*
 * A063731  Numbers k such that Sum_{j=1..k} sigma(j) divides Product_{j=1..k}
 *          sigma(j).
 * https://oeis.org/A063731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) is the nth positive integer k such that Σ_{j=1}^k σ(j)
 *            divides Π_{j=1}^k σ(j)
 * Range:     u8 n <= 13, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, px_hash, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/factor_u64.h"
#include "../utils/px_hash.h"

/* Hash map entry for prime exponents of the product Π σ(j). */
typedef struct {
    uint64_t key;
    uint64_t val;
} px_entry;

static px_entry *px_map      = NULL;
static size_t   px_map_cap   = 0;
static size_t   px_map_size  = 0;
#define PX_MAP_INIT_CAP 16

/* Expand the hash map. */
static void px_map_expand(void) {
    size_t new_cap = px_map_cap ? px_map_cap * 2 : PX_MAP_INIT_CAP;
    px_entry *new_map = (px_entry *)calloc(new_cap, sizeof(px_entry));
    if (!new_map) exit(EXIT_FAILURE);
    size_t mask = new_cap - 1;
    for (size_t i = 0; i < px_map_cap; ++i) {
        if (px_map[i].key) {
            uint64_t k = px_map[i].key;
            uint64_t v = px_map[i].val;
            size_t idx = px_hash(k) & mask;
            while (new_map[idx].key) idx = (idx + 1) & mask;
            new_map[idx].key = k;
            new_map[idx].val = v;
        }
    }
    free(px_map);
    px_map      = new_map;
    px_map_cap  = new_cap;
}

/* Increment exponent of a prime in the map. */
static void px_map_inc(uint64_t key, uint64_t delta) {
    if (px_map_cap == 0) px_map_expand();
    size_t mask = px_map_cap - 1;
    size_t idx  = px_hash(key) & mask;
    while (px_map[idx].key && px_map[idx].key != key)
        idx = (idx + 1) & mask;
    if (px_map[idx].key == key) {
        px_map[idx].val += delta;
    } else {
        px_map[idx].key = key;
        px_map[idx].val = delta;
        px_map_size++;
        if (px_map_size * 3 > px_map_cap * 2) px_map_expand();
    }
}

/* Get exponent of a prime in the map (0 if absent). */
static uint64_t px_map_get(uint64_t key) {
    if (!px_map_cap) return 0;
    size_t mask = px_map_cap - 1;
    size_t idx  = px_hash(key) & mask;
    while (px_map[idx].key && px_map[idx].key != key)
        idx = (idx + 1) & mask;
    return px_map[idx].key ? px_map[idx].val : 0;
}

/* Dynamic array of found k-values. */
static seq_t *terms      = NULL;
static size_t term_cnt   = 0;
static size_t term_cap   = 0;

static void terms_expand(void) {
    size_t new_cap = term_cap ? term_cap * 2 : 64;
    terms = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
    if (!terms) exit(EXIT_FAILURE);
    term_cap = new_cap;
}

static void terms_append(seq_t val) {
    if (term_cnt == term_cap) terms_expand();
    terms[term_cnt++] = val;
}

/* Current state for generation. */
static idx_t cur_k      = 0;            /* last processed integer j */
static uint64_t cur_sum = 0;            /* sum of σ(1..cur_k) */

/* Temporary storage for factor_u64. */
static uint64_t pf_primes[32];
static uint64_t pf_exps[32];
static size_t pf_cnt = 0;

/* Generate terms up to index 'need' (1‑based). */
static void generate_until(idx_t need) {
    while (term_cnt < (size_t)need) {
        ++cur_k;

        /* σ(cur_k) */
        seq_t sigma_val = sigma_of(cur_k);
        uint64_t sigma   = (uint64_t)sigma_val;   /* fits in 64 bits for all test sizes */

        /* Update product exponents with σ(cur_k). */
        factor_u64(sigma, pf_primes, pf_exps, &pf_cnt);
        for (size_t i = 0; i < pf_cnt; ++i)
            px_map_inc(pf_primes[i], pf_exps[i]);

        /* Update sum. */
        cur_sum += sigma;

        /* Check divisibility: for each prime in cur_sum
           exponent must not exceed product exponent. */
        factor_u64(cur_sum, pf_primes, pf_exps, &pf_cnt);
        bool ok = true;
        for (size_t i = 0; i < pf_cnt; ++i) {
            if (px_map_get(pf_primes[i]) < pf_exps[i]) { ok = false; break; }
        }
        if (ok) terms_append((seq_t)cur_k);
    }
}

/* Public function required by OEIS framework. */
seq_t a(idx_t n) {
    if (n == 0) return 0;    /* undefined, but never used */
    generate_until(n);
    return terms[n - 1];
}
