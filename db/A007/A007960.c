/*
 * A007960  Positive numbers k with the property that some permutation of the
 *          digits of k is a triangular number.
 * https://oeis.org/A007960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: k is a term iff the multiset of k's digits equals that
 *            of some triangular number T < 10^(digits of k), with T padded by
 *            leading zeros to the same length
 * Range:     u8 n <= 50, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ndigits_u64, sig_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ndigits_u64.h"
#include "../utils/sig_u64.h"

static uint64_t *terms = NULL;
static idx_t terms_len = 0;
static idx_t terms_cap = 0;

static uint64_t *keys = NULL;
static size_t keys_len = 0;
static int keys_d = -1;

static int cmp_u64(const void *p, const void *q) {
    uint64_t x = *(const uint64_t *)p, y = *(const uint64_t *)q;
    return (x > y) - (x < y);
}

static void build_keys(int d) {
    uint64_t lim = 1;
    uint64_t j, t;
    size_t cap = 1024, i, m;
    int i2;
    for (i2 = 0; i2 < d; i2++) lim *= 10;
    free(keys);
    keys = (uint64_t *)malloc(cap * sizeof(uint64_t));
    keys_len = 0;
    for (j = 1;; j++) {
        t = j * (j + 1) / 2;
        if (t >= lim) break;
        if (keys_len == cap) {
            cap *= 2;
            keys = (uint64_t *)realloc(keys, cap * sizeof(uint64_t));
        }
        keys[keys_len++] = sig_u64(t, d);
    }
    qsort(keys, keys_len, sizeof(uint64_t), cmp_u64);
    m = 0;
    for (i = 0; i < keys_len; i++)
        if (m == 0 || keys[m - 1] != keys[i]) keys[m++] = keys[i];
    keys_len = m;
    keys_d = d;
}

static bool in_seq(uint64_t k) {
    uint64_t r = k % 9;
    int d;
    uint64_t key;
    if (r != 0 && r != 1 && r != 3 && r != 6) return false;
    d = ndigits_u64(k);
    if (d != keys_d) build_keys(d);
    key = sig_u64(k, d);
    return bsearch(&key, keys, keys_len, sizeof(uint64_t), cmp_u64) != NULL;
}

seq_t a(idx_t n) {
    uint64_t k;
    if (terms == NULL) {
        terms_cap = 1024;
        terms = (uint64_t *)malloc(terms_cap * sizeof(uint64_t));
        terms_len = 0;
    }
    k = terms_len > 0 ? terms[terms_len - 1] : 0;
    while (terms_len < n) {
        k++;
        if (in_seq(k)) {
            if (terms_len == terms_cap) {
                terms_cap *= 2;
                terms = (uint64_t *)realloc(terms, terms_cap * sizeof(uint64_t));
            }
            terms[terms_len++] = k;
        }
    }
    return (seq_t)terms[n - 1];
}
