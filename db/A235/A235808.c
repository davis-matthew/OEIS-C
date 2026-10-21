/*
 * A235808  Numbers k such that k^3 has one or more occurrences of exactly six
 *          different digits.
 * https://oeis.org/A235808
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 607, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th integer k such that k^3 contains exactly six distinct
 * decimal digits (OEIS A235808).  The function builds the sequence lazily
 * and stores it in a static cache.  It works for the integer types
 * (uint8_t … uint64_t, unsigned __int128) compiled in C11, as well as for
 * arbitrary‑precision integer types compiled in C++17.
 */

#include <stdlib.h>
#include <stdbool.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Static state for the cache */
typedef struct {
#ifdef __cplusplus
    std::vector<seq_t> vec;
#else
    seq_t *data;
    size_t len;
    size_t cap;
#endif
    uint64_t last_k;          /* last integer tested (k) */
} cache_state_t;

static cache_state_t cache = { .last_k = 0 };

#ifdef __cplusplus
/* Accessors for C++ build */
static inline size_t cache_len(void)   { return cache.vec.size(); }
static inline void cache_push_back(seq_t v) { cache.vec.push_back(v); }
static inline seq_t cache_at(size_t i) { return cache.vec[i]; }
#else
/* Accessors for C build */
static inline size_t cache_len(void)   { return cache.len; }
static inline void cache_push_back(seq_t v) {
    if (cache.len == cache.cap) {
        size_t newcap = cache.cap ? cache.cap * 2 : 4;
        while (newcap <= cache.len) newcap *= 2;
        seq_t *newbuf = (seq_t *)realloc(cache.data, newcap * sizeof(seq_t));
        if (!newbuf) exit(1);
        cache.data = newbuf;
        cache.cap = newcap;
    }
    cache.data[cache.len++] = v;
}
static inline seq_t cache_at(size_t i) { return cache.data[i]; }
#endif

/* Return true iff x (as decimal) contains exactly six distinct digits. */
static bool has_six_distinct_digits(seq_t x) {
    unsigned char seen[10] = {0};
    if (x == (seq_t)0) {
        seen[0] = 1;
    } else {
        seq_t ten = (seq_t)10;
        while (x > (seq_t)0) {
            seq_t digit = x % ten;
            seen[(unsigned char)digit] = 1;
            x /= ten;
        }
    }
    int cnt = 0;
    for (int i = 0; i < 10; ++i)
        if (seen[i]) ++cnt;
    return cnt == 6;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n < (idx_t)1) return (seq_t)0;   /* undefined for n < 1 */

    /* Generate terms until we have at least n cached */
    while (cache_len() < (size_t)n) {
        ++cache.last_k;
        seq_t k = (seq_t)cache.last_k;
        seq_t cube = k * k * k;        /* compute k^3 */
        if (has_six_distinct_digits(cube)) {
            cache_push_back(k);
        }
    }
    return cache_at((size_t)n - 1);
}
