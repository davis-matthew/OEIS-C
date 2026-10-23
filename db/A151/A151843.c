/*
 * A151843  a(0)=0; a(1)=0; a(2)=0; for n>=3 if n=2^i + j with 0<=j<2^i then
 *          a(n)=a(j) + a(j + 1) except we add 1 if j=2^i-1.
 * https://oeis.org/A151843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0; a(1)=0; a(2)=0; for n>=3 if n = 2^i + j with 0<=j<2^i
 *            then a(n) = a(j) + a(j+1), plus 1 if j = 2^i - 1
 * Range:     u8 n <= 4088, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static unsigned char *computed = NULL;
static size_t cap = 0;

/* Ensure the cache can store values up to index n.  Initialise base
   entries a(0), a(1), a(2) to 0. */
static void ensure_cache(idx_t n) {
    size_t needed = (size_t)n + 1;
    if (needed <= cap) return;
    size_t newcap = cap > 0 ? cap * 2 : 4;
    while (newcap < needed) newcap <<= 1;
    cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    computed = (unsigned char *)realloc(computed, newcap * sizeof(unsigned char));
    for (size_t i = cap; i < newcap; ++i) computed[i] = 0;
    cap = newcap;
    /* Initialise the base cases */
    if (cap > 0)      { cache[0] = 0; computed[0] = 1; }
    if (cap > 1)      { cache[1] = 0; computed[1] = 1; }
    if (cap > 2)      { cache[2] = 0; computed[2] = 1; }
}

/* Integer floor of log2 for idx_t values.  n>=1. */
static unsigned int floor_log2_idx(idx_t v) {
    unsigned int pos = 0;
    while (((idx_t)1ULL << (pos + 1)) <= v) ++pos;
    return pos;
}

seq_t a(idx_t n) {
    if (n <= 2u) return 0;          /* a(0)=a(1)=a(2)=0 */
    ensure_cache(n);
    size_t idx = (size_t)n;
    if (computed[idx]) return cache[idx];

    unsigned int i = floor_log2_idx(n);
    idx_t pow2 = ((idx_t)1ULL << i);        /* 2^i */
    idx_t j = n - pow2;                    /* 0 <= j < 2^i */
    int add1 = (j == pow2 - 1) ? 1 : 0;

    seq_t val1 = a(j);
    seq_t val2 = a(j + 1);
    seq_t res = val1 + val2;
    if (add1) ++res;

    cache[idx] = res;
    computed[idx] = 1;
    return res;
}
