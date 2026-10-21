/*
 * A077159  Triangle in which n-th row contains smallest n numbers coprime to
 *          n which have not occurred earlier, with a(1)=2.
 * https://oeis.org/A077159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) is obtained by building rows of the triangle.
   Each row n contains the smallest n unused numbers that are coprime to n.
   The flattened sequence is returned by a(idx_t n). */

/* Necessary includes */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"
#if defined(__cplusplus)
#include <vector>
#endif

/* ----------------- cache of generated terms ----------------- */
#if defined(__cplusplus)
static std::vector<seq_t> cache_vec;   /* C++ build */
#else
static seq_t *cache = NULL;            /* C build */
static size_t cache_cap = 0;
#endif
static size_t cache_sz = 0;            /* number of cached terms */

/* ----------------- bitmap of used numbers ----------------- */
static unsigned char *used = NULL;     /* 0 → not used, 1 → used */
static size_t used_cap = 0;            /* capacity of used[] */

/* ----------------- helper for bitmap ----------------- */
static void ensure_used_cap(size_t n)
{
    if (used_cap <= n) {
        size_t new_cap = used_cap ? used_cap << 1 : 128;
        while (new_cap <= n) new_cap <<= 1;
        unsigned char *tmp = (unsigned char *)realloc(used, new_cap * sizeof(unsigned char));
        if (!tmp) abort();
        used = tmp;
        size_t i;
        for (i = used_cap; i < new_cap; ++i) used[i] = 0;
        used_cap = new_cap;
    }
}

/* ----------------- helper for cache ----------------- */
#if defined(__cplusplus)
static void ensure_cache_cap(size_t /*need*/)   /* std::vector grows automatically */
{
}
static void add_to_cache(seq_t val)
{
    cache_vec.push_back(val);
    cache_sz = cache_vec.size();
}
static seq_t get_cache_val(idx_t idx)
{
    return cache_vec[(size_t)(idx)];
}
#else
static void ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        while (new_cap < need) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
}
static void add_to_cache(seq_t val)
{
    cache[cache_sz++] = val;
}
static seq_t get_cache_val(idx_t idx)
{
    return cache[(size_t)(idx)];
}
#endif

/* ----------------- generate rows until at least n terms -------------- */
static void generate_up_to(idx_t n)
{
    if (cache_sz >= (size_t)n) return;          /* already enough */

    /* initialise first term a(1)=2 */
    if (cache_sz == 0) {
        ensure_cache_cap(1);
#if defined(__cplusplus)
        cache_vec.reserve(1);
        add_to_cache((seq_t)2);
#else
        cache[0] = (seq_t)2;
        cache_sz = 1;
#endif
        ensure_used_cap(2);
        used[2] = 1;
    }

    /* rows_done counts how many rows have been processed already */
    static idx_t rows_done = 1;

    /* produce rows until cache_sz >= n */
    while (cache_sz < (size_t)n) {
        idx_t r = rows_done + 1;                /* next row number (1‑based) */
        seq_t rseq = (seq_t)r;
        seq_t placed = 0;                       /* items placed in this row */
        seq_t cand = 1;                         /* candidate value to test */

        while (placed < r) {
            size_t cand_sz = (size_t)cand;
            if (cand_sz >= used_cap)
                ensure_used_cap(cand_sz);

            /* check unused and coprime to r */
            if (!used[cand_sz] && gcd(cand, rseq) == 1) {
                ensure_cache_cap(cache_sz + 1);
                add_to_cache(cand);
                used[cand_sz] = 1;
                ++placed;
            }
            ++cand;                              /* move to next integer */
        }
        ++rows_done;
    }
}

/* ----------------- exported function ----------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined for n=0, return 0 */
    generate_up_to(n);
    return get_cache_val(n - 1);
}
