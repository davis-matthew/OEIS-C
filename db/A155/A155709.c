/*
 * A155709  Intersection of A154777 and A155716: N = a^2 + 2b^2 = c^2 + 6d^2
 *          for some positive integers a,b,c,d.
 * https://oeis.org/A155709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A155709: numbers N such that N = a² + 2b² = c² + 6d² with a,b,c,d ≥ 1. */

#ifndef _SEQ_A155709_H_
#define _SEQ_A155709_H_

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* integer square‑root of an unsigned 64‑bit value */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = (n >> 1) + 1;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* comparison function for qsort (C only) */
#if !defined(__cplusplus)
static int cmp_seq(const void *p1, const void *p2)
{
    seq_t a = *(const seq_t *)p1;
    seq_t b = *(const seq_t *)p2;
    return (a < b) ? -1 : (a > b) ? 1 : 0;
}
#endif

/* cache of pre‑computed intersection numbers */
#if defined(__cplusplus)
#include <vector>
static std::vector<seq_t> cache;
#else
static seq_t *cache        = NULL;
static size_t cached_count = 0;
static size_t cached_cap   = 0;
#endif

/* ensure at least n_needed terms are stored in the cache */
static void ensure_terms(idx_t n_needed)
{
    if (n_needed == 0)
        return;

#if defined(__cplusplus)
    size_t needed = (size_t)n_needed;
    if (cache.size() >= needed)
        return;
#else
    size_t needed = (size_t)n_needed;
    if (cached_count >= needed)
        return;
#endif

    idx_t bound = 1;

    while (true) {
        /* ---- generate listA : a² + 2b² <= bound (a,b >= 1) ---- */
#if defined(__cplusplus)
        std::vector<seq_t> listA;
        std::vector<seq_t> listB;
#else
        seq_t *listA = NULL;
        size_t sizeA = 0, capA = 0;
        seq_t *listB = NULL;
        size_t sizeB = 0, capB = 0;
#endif

        idx_t a_max = isqrt_u64(bound);
        for (idx_t a = 1; a <= a_max; ++a) {
            idx_t aa = a * a;
            idx_t rem = bound - aa;
            if (rem < 2)
                continue;
            idx_t b_max = isqrt_u64(rem / 2);
            for (idx_t b = 1; b <= b_max; ++b) {
#if defined(__cplusplus)
                listA.push_back((seq_t)aa + (seq_t)2 * b * b);
#else
                if (sizeA == capA) {
                    capA = capA == 0 ? 32 : capA << 1;
                    listA = (seq_t *)realloc(listA, capA * sizeof(seq_t));
                }
                listA[sizeA++] = (seq_t)aa + (seq_t)2 * b * b;
#endif
            }
        }

        /* ---- generate listB : c² + 6d² <= bound (c,d >= 1) ---- */
        idx_t c_max = isqrt_u64(bound);
        for (idx_t c = 1; c <= c_max; ++c) {
            idx_t cc = c * c;
            idx_t rem = bound - cc;
            if (rem < 6)
                continue;
            idx_t d_max = isqrt_u64(rem / 6);
            for (idx_t d = 1; d <= d_max; ++d) {
#if defined(__cplusplus)
                listB.push_back((seq_t)cc + (seq_t)6 * d * d);
#else
                if (sizeB == capB) {
                    capB = capB == 0 ? 32 : capB << 1;
                    listB = (seq_t *)realloc(listB, capB * sizeof(seq_t));
                }
                listB[sizeB++] = (seq_t)cc + (seq_t)6 * d * d;
#endif
            }
        }

#if defined(__cplusplus)
        std::sort(listA.begin(), listA.end());
        std::sort(listB.begin(), listB.end());

        /* intersect two sorted vectors and append new entries to cache */
        size_t i = 0, j = 0;
        while (i < listA.size() && j < listB.size()) {
            if (listA[i] == listB[j]) {
                if (cache.empty() || listA[i] > cache.back())
                    cache.push_back(listA[i]);
                ++i; ++j;
            } else if (listA[i] < listB[j]) {
                ++i;
            } else {
                ++j;
            }
        }
#else
        qsort(listA, sizeA, sizeof(seq_t), cmp_seq);
        qsort(listB, sizeB, sizeof(seq_t), cmp_seq);

        size_t i = 0, j = 0;
        while (i < sizeA && j < sizeB) {
            if (listA[i] == listB[j]) {
                if (cached_count == 0 || listA[i] > cache[cached_count-1]) {
                    if (cached_count == cached_cap) {
                        cached_cap = cached_cap == 0 ? 32 : cached_cap << 1;
                        cache = (seq_t *)realloc(cache, cached_cap * sizeof(seq_t));
                    }
                    cache[cached_count++] = listA[i];
                }
                ++i; ++j;
            } else if (listA[i] < listB[j]) {
                ++i;
            } else {
                ++j;
            }
        }
        free(listA);
        free(listB);
#endif

        /* check if we have gathered enough terms */
#if defined(__cplusplus)
        if (cache.size() >= needed)
            break;
#else
        if (cached_count >= needed)
            break;
#endif

        if (bound > (idx_t)1ULL << 62)   /* safety guard */
            break;
        bound <<= 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* index 0 is unused */
    ensure_terms(n);
#if defined(__cplusplus)
    return cache[(size_t)n - 1];
#else
    return cache[(size_t)n - 1];
#endif
}

#endif /* _SEQ_A155709_H_ */
