/*
 * A031556  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 58.
 * https://oeis.org/A031556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A031556: Numbers k such that the continued fraction for sqrt(k) has an even period
 * and, after deleting the last term of the periodic part, the central term is 58.
 * 
 * Implementation performs a brute‑force search, caching found values.
 * It works for both the integer builds (C11) and arbitrary‑precision builds (C++17).
 * 
 * Complexity: roughly O(number of candidates checked), which is acceptable for n≤10000.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

#ifdef __cplusplus
#include <vector>
#endif

/* ------------------- helper: test one D ------------------- */
static bool satisfies(uint64_t D_val)
{
    seq_t D = D_val;
    seq_t a0 = floor_sqrt(D);

    /* ignore perfect squares */
    if (a0 * a0 == D)
        return false;

    seq_t m = 0;
    seq_t d = 1;
    seq_t a = a0;

#if defined(__cplusplus)
    std::vector<seq_t> terms;          /* holds the periodic terms */
#else
    size_t cap = 16;
    seq_t *terms = (seq_t*)malloc(cap * sizeof(seq_t));
    if (!terms)
        return false;                /* allocation failure, treat as non‑candidate */
    size_t pos = 0;                   /* current fill position */
#endif

    /* generate the periodic part */
    do {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

#if defined(__cplusplus)
        terms.push_back(a);
#else
        if (pos == cap) {            /* grow the array */
            cap <<= 1;
            seq_t *tmp = (seq_t*)realloc(terms, cap * sizeof(seq_t));
            if (!tmp) {
                free(terms);
                return false;
            }
            terms = tmp;
        }
        terms[pos++] = a;
#endif
    } while (d != 1);                  /* end of period */

#if defined(__cplusplus)
    const size_t len = terms.size();
#else
    const size_t len = pos;
#endif

    /* period must be even */
    if (len & 1) {
#if !defined(__cplusplus)
        free(terms);
#endif
        return false;
    }

    /* central term after deleting last = terms[len/2 - 1] */
    seq_t mid = terms[(len / 2) - 1];
#if !defined(__cplusplus)
    free(terms);
#endif
    return mid == (seq_t)58;
}

/* ------------------- main sequence function ------------------- */
seq_t a(idx_t n)
{
    /* static state for caching */
#ifdef __cplusplus
    static std::vector<seq_t> cache;
    static uint64_t next_k = 2;            /* start checking from 2 */
    /* fetch/cache until we have n terms */
    while (cache.size() < (size_t)n) {
        uint64_t k = next_k++;
        if (satisfies(k))
            cache.push_back((seq_t)k);
    }
    return cache[(size_t)n - 1];
#else
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_k = 2;            /* start checking from 2 */

    /* grow cache array only when needed */
    while (cache_len < (size_t)n) {
        uint64_t k = next_k++;
        if (satisfies(k)) {
            if (cache_len == cache_cap) {  /* expand storage */
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)         /* allocation failure: abort */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }
    return cache[(size_t)n - 1];
#endif
}
