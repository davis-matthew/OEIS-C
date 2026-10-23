/*
 * A162554  Triples of numbers {n1 < n2 < n3} such that {n1, n2, n3}^2
 *          together use exactly 9 decimals 1..9, sorted first by n1, then by
 *          n2, and by n3.
 * https://oeis.org/A162554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn, base
 * Formula:   a(n) enumerates the triples of numbers * n1 < n2 < n3 such that
 *            the decimal digits of n1², n2², n3² * together form a
 *            permutation of 1..9. The sequence consists of * the terms of all
 *            such triples sorted first by n1, then n2, then n3. * The
 *            returned value is the n-th element of that list
 * Range:     u8 n <= 2, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   first_index_gt, valid_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/first_index_gt.h"
#include "../utils/valid_square.h"

static const int ALL_MASK = 0x3FE;            /* bits 1..9 set */
#define MAX_N 31622                           /* sqrt(10^9-1) */
#define MAX_TRIPLES 100
#define MASK_COUNT 1024

typedef struct {
    uint64_t n;
    int mask;
} Candidate;

static Candidate candidates[MAX_N + 1];
static size_t cand_cnt = 0;

/* list of candidate indices for each mask */
typedef struct {
    size_t len;
    size_t cap;
    size_t *idx;
} MaskList;
static MaskList mask_lists[MASK_COUNT] = {0};

static void add_to_masklist(int mask, size_t idx) {
    MaskList *ml = &mask_lists[mask];
    if (ml->len == ml->cap) {
        size_t newcap = ml->cap ? ml->cap * 2 : 4;
        ml->idx = (size_t *)realloc(ml->idx, newcap * sizeof(size_t));
        ml->cap = newcap;
    }
    ml->idx[ml->len++] = idx;
}

static void build_candidates(void) {
    for (uint64_t n = 1; n <= MAX_N; ++n) {
        uint64_t sq = (uint64_t)n * (uint64_t)n;
        int mask;
        if (!valid_square(sq, &mask))
            continue;
        candidates[cand_cnt].n = n;
        candidates[cand_cnt].mask = mask;
        add_to_masklist(mask, cand_cnt);
        ++cand_cnt;
    }
}

static seq_t triples_n1[MAX_TRIPLES];
static seq_t triples_n2[MAX_TRIPLES];
static seq_t triples_n3[MAX_TRIPLES];
static size_t triples_cnt = 0;

static void compute_triples(void) {
    build_candidates();
    for (size_t i = 0; i < cand_cnt; ++i) {
        int mask_i = candidates[i].mask;
        for (size_t j = i + 1; j < cand_cnt; ++j) {
            int mask_j = candidates[j].mask;
            if (mask_i & mask_j)
                continue;
            int combined = mask_i | mask_j;
            int comp = ALL_MASK ^ combined;
            if (comp == 0)
                continue;
            MaskList *ml = &mask_lists[comp];
            if (ml->len == 0)
                continue;
            size_t pos = first_index_gt(ml->idx, ml->len, j);
            for (size_t p = pos; p < ml->len; ++p) {
                size_t k = ml->idx[p];
                triples_n1[triples_cnt] = (seq_t)candidates[i].n;
                triples_n2[triples_cnt] = (seq_t)candidates[j].n;
                triples_n3[triples_cnt] = (seq_t)candidates[k].n;
                ++triples_cnt;
                if (triples_cnt >= MAX_TRIPLES)
                    return;
            }
        }
    }
}

extern seq_t a(idx_t n) {
    static bool initialized = false;
    if (!initialized) {
        compute_triples();
        initialized = true;
    }
    if (n == 0)
        return 0;
    size_t idx = (size_t)(n - 1);
    size_t t = idx / 3;
    if (t >= triples_cnt)
        return 0;
    size_t pos = idx % 3;
    if (pos == 0)
        return triples_n1[t];
    if (pos == 1)
        return triples_n2[t];
    return triples_n3[t];
}
