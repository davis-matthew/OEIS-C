/*
 * A045010  Numbers whose base-3 representation contains exactly four 0's and
 *          one 2.
 * https://oeis.org/A045010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) generates numbers whose base‑3 representation contains
 *            exactly four 0's and one 2, sorted increasingly
 * Range:     u8 n <= 1, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;       /* array of terms */
static size_t cache_len = 0;      /* number of terms stored */
static size_t cache_cap = 0;      /* capacity of cache array */
static size_t max_gen_len = 0;    /* highest length L for which we generated numbers */
static bool cache_sorted = false;/* flag saying cache is sorted */

/* compare function for qsort */
static int cmp_seq(const void *a, const void *b)
{
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* push a value into the cache, reallocating if needed */
static void cache_push(seq_t value)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2ULL : 64ULL;
        seq_t *tmp = seq_realloc(cache, cache_cap, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = value;
    cache_sorted = false;
}

/* generate all numbers with given length L (≥5) and add them
   to the cache.  L is the number of base‑3 digits. */
static void generate_L(size_t L)
{
    if (L < 5) return;

    for (size_t p0 = 0; p0 <= L - 5; ++p0)
    for (size_t p1 = p0 + 1; p1 <= L - 4; ++p1)
    for (size_t p2 = p1 + 1; p2 <= L - 3; ++p2)
    for (size_t p3 = p2 + 1; p3 <= L - 2; ++p3)
    for (size_t p4 = p3 + 1; p4 <= L - 1; ++p4) {
        size_t pos[5] = {p0, p1, p2, p3, p4};

        /* for each choice of the 2’s position among the 5 */
        for (int j = 0; j < 5; ++j) {
            seq_t val = 0;
            bool skip = false;

            for (size_t t = 0; t < L; ++t) {
                size_t dpos = L - 1 - t;   /* digits counted from MSB */
                int digit;

                if (dpos == pos[j]) {
                    digit = 2;
                } else {
                    /* check if dpos is one of the other zero positions */
                    bool isZero = false;
                    for (int k = 0; k < 5; ++k) {
                        if (k == j) continue;
                        if (pos[k] == dpos) { isZero = true; break; }
                    }
                    digit = isZero ? 0 : 1;
                }

                /* skip numbers with a leading 0 */
                if (t == 0 && digit == 0) { skip = true; break; }

                val = val * ((seq_t)3) + ((seq_t)digit);
            }
            if (skip) continue;
            cache_push(val);
        }
    }
}

/* Ensure that the cache contains at least n terms,
   generating more lengths if necessary. */
static void ensure_cache(idx_t n)
{
    if (!cache) {
        cache_cap = 64;
        cache = seq_alloc(cache_cap);
        if (!cache) abort();
    }

    size_t needed = (size_t)n;
    while (cache_len < needed) {
        size_t L = max_gen_len + 1;
        generate_L(L);
        max_gen_len = L;
    }

    if (!cache_sorted) {
        qsort(cache, cache_len, sizeof(seq_t), cmp_seq);
        cache_sorted = true;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset 1: a(1) is first term */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
