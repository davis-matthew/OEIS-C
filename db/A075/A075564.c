/*
 * A075564  Rearrangement of natural numbers such that n-th partial sum is ==
 *          n (mod c(n)). where c(n) is the n-th composite number.
 * https://oeis.org/A075564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A075564:  Rearrangement of natural numbers such that
   n‑th partial sum S(n) satisfies S(n) ≡ n (mod c(n)),
   where c(n) is the n‑th composite number. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/is_prime.h"

/* Cached data structures */
static seq_t *terms   = NULL;   /* terms[1 .. max_cached] */
static size_t terms_cap = 0;
static idx_t max_cached = 0;
static seq_t current_sum = 0;    /* S(max_cached) */

static bool *used   = NULL;      /* used[i] == true if i has been used */
static size_t used_cap = 0;

static uint64_t *comps    = NULL;/* comps[0 .. comps_cnt-1] = c(n) */
static size_t comps_cap = 0;
static size_t comps_cnt = 0;
static uint64_t comps_next = 4;

/* Ensure that 'used' can address index 'needed' (inclusive). */
static void ensure_used_capacity(idx_t needed)
{
    if (!used) {
        size_t sz = needed + 1;
        if (sz < 17) sz = 17;
        used = (bool*)calloc(sz, sizeof(bool));
        if (!used) exit(1);
        used_cap = sz;
        return;
    }
    while ((size_t)needed + 1 > used_cap) {
        size_t new_cap = used_cap * 2;
        if ((size_t)needed + 1 > new_cap) new_cap = (size_t)needed + 1;
        bool *tmp = (bool*)realloc(used, new_cap * sizeof(bool));
        if (!tmp) exit(1);
        memset(tmp + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
        used = tmp;
        used_cap = new_cap;
    }
}

/* Ensure that 'terms' can address index 'needed' (inclusive). */
static void ensure_terms_capacity(idx_t needed)
{
    if (!terms) {
        size_t sz = needed + 1;
        if (sz < 17) sz = 17;
        terms = (seq_t*)calloc(sz, sizeof(seq_t));
        if (!terms) exit(1);
        terms_cap = sz;
        return;
    }
    while ((size_t)needed + 1 > terms_cap) {
        size_t new_cap = terms_cap * 2;
        if ((size_t)needed + 1 > new_cap) new_cap = (size_t)needed + 1;
        seq_t *tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Grow the composite list until it contains at least 'need' entries. */
static uint64_t nth_composite(idx_t need)
{
    if (comps_cnt >= (size_t)need) return comps[need - 1];

    if (!comps) {
        size_t sz = 64;
        comps = (uint64_t*)calloc(sz, sizeof(uint64_t));
        if (!comps) exit(1);
        comps_cap = sz;
    }

    while (comps_cnt < (size_t)need) {
        /* find next composite */
        while (1) {
            if (!is_prime(comps_next)) {
                comps[comps_cnt++] = comps_next;
                ++comps_next;
                break;
            }
            ++comps_next;
        }
        if (comps_cnt == comps_cap) {
            size_t new_cap = comps_cap * 2;
            if (new_cap == 0) new_cap = 64;
            uint64_t *tmp = (uint64_t*)realloc(comps, new_cap * sizeof(uint64_t));
            if (!tmp) exit(1);
            comps = tmp;
            comps_cap = new_cap;
        }
    }
    return comps[need - 1];
}

/* Main sequence routine */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined for n < 1; guard */
    if (n <= max_cached) return terms[n];

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        uint64_t comp_uint = nth_composite(i);
        seq_t  mod      = (seq_t)comp_uint;
        seq_t  i_mod    = (seq_t)(i % comp_uint);
        seq_t  remSum   = current_sum % mod;
        seq_t  target   = (i_mod + mod - remSum) % mod;
        seq_t  candidate = (target == 0 ? mod : target);

        /* Find the smallest unused integer ≡ target (mod mod). */
        for (;;) {
            idx_t idx = (idx_t)candidate;
            if (idx >= used_cap) ensure_used_capacity(idx);
            if (!used[idx]) break;
            candidate += mod;
        }

        idx_t cand_idx = (idx_t)candidate;
        ensure_used_capacity(cand_idx);
        used[cand_idx] = true;

        ensure_terms_capacity(i);
        terms[i] = candidate;
        current_sum += candidate;
    }

    max_cached = n;
    return terms[n];
}
