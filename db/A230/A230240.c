/*
 * A230240  Values of N for which the equation x^2 - 9*y^2 = N has integer
 *          solutions.
 * https://oeis.org/A230240
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 986 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A230240: N for which x^2 - 9*y^2 = N has integer solutions.
 * A number N satisfies this iff there exist integers a,b such that
 * N = a*b and a ≡ b (mod 6).  We generate the sequence by testing
 * each non‑negative integer for this property and store the found
 * values in a static cache so subsequent calls are fast.
 */ 

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *cache = NULL;          /* array of found N's               */
static size_t cache_size = 0;        /* number of elements in cache     */
static size_t cache_cap  = 0;        /* allocated capacity              */
static idx_t  next_n      = 0;        /* next candidate N to test        */

/* Test whether N can be represented as x^2 - 9*y^2. */
static bool is_representable(seq_t N)
{
    if (N == 0) return true;              /* 0 = 0^2 - 9*0^2 */
    seq_t limit = floor_sqrt(N);
    idx_t lim = (idx_t)limit;              /* safe for expected size */

    for (idx_t a = 1; a <= lim; ++a) {
        if (N % (seq_t)a == 0) {
            seq_t b = N / (seq_t)a;
            seq_t diff = b - (seq_t)a;
            if ((diff % (seq_t)6) == 0) return true;
        }
    }
    return false;
}

/* Return the nth term (1‑based index).  Sequence starts with a(1)=0. */
seq_t a(idx_t n)
{
    if (cache == NULL) {                  /* lazy initialization      */
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        cache_size = 0;
        next_n = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_size < n) {
        seq_t cand = (seq_t)next_n;
        if (is_representable(cand)) {
            if (cache_size >= cache_cap) {
                cache_cap *= 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                cache = tmp;
            }
            cache[cache_size++] = cand;
        }
        next_n++;
    }
    return cache[n - 1];
}
