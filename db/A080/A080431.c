/*
 * A080431  Even numbers such that all a(i)*a(j) are distinct.
 * https://oeis.org/A080431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Greedy construction of even numbers a(1)=2, then for n>1 * pick
 *            the smallest even integer e > a(n‑1) such that for all * i<j,
 *            the products a(i)*a(j) are distinct. This algorithm maintains *
 *            a hash set of all pairwise products a(i)*a(j) with i<j that have
 *            * already appeared. Self products a(k)*a(k...
 * Range:     u8 n <= 41, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   power_of_two_ge
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/power_of_two_ge.h"

#if defined(__SIZEOF_INT128__)
typedef __uint128_t prod_t;
#else
typedef unsigned long long prod_t;
#endif

/* 64‑bit hash of a 128‑bit product.  */
static uint64_t prod_hash(prod_t v)
{
#if defined(__SIZEOF_INT128__)
    v ^= v >> 33;
    v *= (prod_t)0xff51afd7ed558ccdULL;
    v ^= v >> 33;
    v *= (prod_t)0xc4ceb9fe1a85ec53ULL;
    v ^= v >> 33;
#else
    v ^= v >> 33;
    v *= 0xff51afd7ed558ccdULL;
    v ^= v >> 33;
    v *= 0xc4ceb9fe1a85ec53ULL;
    v ^= v >> 33;
#endif
    return (uint64_t)v;
}

typedef struct {
    prod_t *table;
    size_t capacity;
    size_t used;
} ProdSet;

static void prodset_init(ProdSet *ps, size_t cap)
{
    ps->capacity = power_of_two_ge(cap);
    ps->table = (prod_t *)calloc(ps->capacity, sizeof(prod_t));
    ps->used = 0;
}

static void prodset_free(ProdSet *ps)
{
    free(ps->table);
    ps->table = NULL;
    ps->capacity = 0;
    ps->used = 0;
}

static void prodset_rehash(ProdSet *ps)
{
    size_t old_cap = ps->capacity;
    prod_t *old_tbl = ps->table;
    size_t new_cap = old_cap << 1;
    prod_t *new_tbl = (prod_t *)calloc(new_cap, sizeof(prod_t));

    ps->capacity = new_cap;
    ps->table = new_tbl;
    ps->used = 0;

    for (size_t i = 0; i < old_cap; ++i) {
        prod_t key = old_tbl[i];
        if (key != 0) {
            uint64_t h = prod_hash(key) & (new_cap - 1);
            while (new_tbl[h] != 0) h = (h + 1) & (new_cap - 1);
            new_tbl[h] = key;
            ps->used++;
        }
    }
    free(old_tbl);
}

static bool prodset_contains(const ProdSet *ps, prod_t key)
{
    if (key == 0) return false;
    uint64_t h = prod_hash(key) & (ps->capacity - 1);
    while (true) {
        prod_t cur = ps->table[h];
        if (cur == 0) return false;
        if (cur == key) return true;
        h = (h + 1) & (ps->capacity - 1);
    }
}

static void prodset_insert(ProdSet *ps, prod_t key)
{
    if (key == 0) return;
    if (ps->used > (ps->capacity * 3) / 4) prodset_rehash(ps);

    uint64_t h = prod_hash(key) & (ps->capacity - 1);
    while (true) {
        if (ps->table[h] == 0) {
            ps->table[h] = key;
            ps->used++;
            return;
        } else if (ps->table[h] == key) {
            return;
        }
        h = (h + 1) & (ps->capacity - 1);
    }
}

seq_t a(idx_t n)
{
    static seq_t *terms = NULL;
    static size_t terms_cap = 0;
    static size_t terms_len = 0;
    static ProdSet prodset;
    static bool prodset_inited = false;

    if (!prodset_inited) {
        prodset_init(&prodset, 1 << 20);  /* ~1M entries */
        prodset_inited = true;
    }

    if (n == 0) return 0; /* offset 1: behaviour before a(1) is not needed */

    /* Grow terms array as needed. */
    if (terms_len < (size_t)n) {
        if (terms_len == 0) {
            terms_cap = 16;
            terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
        } else if (terms_len == terms_cap) {
            terms_cap <<= 1;
            terms = (seq_t *)realloc(terms, terms_cap * sizeof(seq_t));
        }

        for (; terms_len < (size_t)n; ++terms_len) {
            seq_t cand;
            if (terms_len == 0)
                cand = (seq_t)2;
            else
                cand = terms[terms_len - 1] + (seq_t)2;

            /* Find the smallest even cand that keeps all pairwise products distinct. */
            while (1) {
                bool ok = true;

                /* Check cand against all existing terms (i<j). */
                for (size_t i = 0; i < terms_len; ++i) {
                    prod_t prod = (prod_t)cand * (prod_t)terms[i];
                    if (prodset_contains(&prodset, prod)) {
                        ok = false;
                        break;
                    }
                }

                if (ok) {
                    /* Insert new products into the set. */
                    for (size_t i = 0; i < terms_len; ++i) {
                        prod_t prod = (prod_t)cand * (prod_t)terms[i];
                        prodset_insert(&prodset, prod);
                    }
                    terms[terms_len] = cand;
                    break;
                }

                /* Advance to the next even candidate. */
                cand += (seq_t)2;
            }
        }
    }

    return terms[n - 1];
}
