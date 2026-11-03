/*
 * A202665  Number of -n..n arrays of 5 elements with first through fourth
 *          differences also in -n..n.
 * https://oeis.org/A202665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3)+2*a(n-4)+a(n-5)-4*a(n-7)-3*a(n-8)-4*a(n-9) *
 *            +2*a(n-11)+5*a(n-12)+5*a(n-13)+2*a(n-14)-4*a(n-16)-3*a(n-17) *
 *            -4*a(n-18)+a(n-20)+2*a(n-21)+2*a(n-22)-a(n-25) * on indices n ≥
 *            26 with a(1)…a(25) given by direct enumeration
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 73, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 187 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* of 5 elements in [-n..n] that satisfy the differences constraints.  */
static seq_t exact_compute(idx_t n)
{
    int N = (int)n;           /* n <= 25, so int is safe */
    int neg = -N, pos = N;
    seq_t count = 0;

    for (int a0 = neg; a0 <= pos; ++a0) {
        for (int a1 = neg; a1 <= pos; ++a1) {
            int d1 = a1 - a0;
            if (d1 < -N || d1 > N) continue;
            for (int a2 = neg; a2 <= pos; ++a2) {
                int d2 = a2 - a1;
                if (d2 < -N || d2 > N) continue;
                int f2_0 = d2 - d1;
                if (f2_0 < -N || f2_0 > N) continue;
                for (int a3 = neg; a3 <= pos; ++a3) {
                    int d3 = a3 - a2;
                    if (d3 < -N || d3 > N) continue;
                    int f2_1 = d3 - d2;
                    if (f2_1 < -N || f2_1 > N) continue;
                    int f3_0 = f2_1 - f2_0;
                    if (f3_0 < -N || f3_0 > N) continue;
                    for (int a4 = neg; a4 <= pos; ++a4) {
                        int d4 = a4 - a3;
                        if (d4 < -N || d4 > N) continue;
                        int f2_2 = d4 - d3;
                        if (f2_2 < -N || f2_2 > N) continue;
                        int f3_1 = f2_2 - f2_1;
                        if (f3_1 < -N || f3_1 > N) continue;
                        int f4   = f3_1 - f3_0;
                        if (f4 < -N || f4 > N) continue;
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

/* static cache of computed values, 1‑based */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cached_n = 0;

/* ------------------------------------------------------------------ */
/* Compute a(n) for n <= 25 by explicit enumeration of all arrays      */

/* ------------------------------------------------------------------ */
/* Compute a(n) for n ≥ 26 using the linear recurrence.               */
static seq_t recurrence_compute(idx_t m)
{
    /* All operands are non‑negative, so unsigned arithmetic is safe. */
    seq_t res = 0;
    res += cache[m - 3] * 2;
    res += cache[m - 4] * 2;
    res += cache[m - 5];
    res -= cache[m - 7] * 4;
    res -= cache[m - 8] * 3;
    res -= cache[m - 9] * 4;
    res += cache[m - 11] * 2;
    res += cache[m - 12] * 5;
    res += cache[m - 13] * 5;
    res += cache[m - 14] * 2;
    res -= cache[m - 16] * 4;
    res -= cache[m - 17] * 3;
    res -= cache[m - 18] * 4;
    res += cache[m - 20];
    res += cache[m - 21] * 2;
    res += cache[m - 22] * 2;
    res -= cache[m - 25];
    return res;
}

/* ------------------------------------------------------------------ */
/* Ensure that cache has values up to `needed`.                        */
static void grow_cache(idx_t needed)
{
    if (needed <= cached_n)
        return;

    size_t new_cap = (size_t)(needed + 1);   /* indices 0..needed */
    if (new_cap > cache_cap) {
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        if (!new_cache) abort();            /* out of memory */
        cache = new_cache;
        cache_cap = new_cap;
        /* zero-initialize new space */
        for (size_t i = cached_n + 1; i < new_cap; ++i)
            cache[i] = 0;
    }

    for (idx_t m = cached_n + 1; m <= needed; ++m) {
        if (m <= 25)
            cache[m] = exact_compute(m);
        else
            cache[m] = recurrence_compute(m);
    }
    cached_n = needed;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                            */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* sequence is defined for n ≥ 1 */
    grow_cache(n);
    return cache[n];
}
