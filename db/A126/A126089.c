/*
 * A126089  Expansion of e.g.f.: (1-2*x)*sqrt(1-4*x).
 * https://oeis.org/A126089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 15, i128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=-4, a(n)=4*n*a(n-1)-12*(2*n-7)*a(n-2) (n>=2) */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/alloc_seq.h"

static seq_t *cache = NULL;   /* dynamic array for memoised values */
static size_t cap = 0;        /* current capacity of cache       */

static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)(n + 1);
    if (needed <= cap) return;
    seq_t *tmp = alloc_seq(needed);
    if (cap > 0) {
        for (size_t i = 0; i < cap; ++i) tmp[i] = cache[i];
    }
    cache = tmp;
    cap = needed;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    for (idx_t i = 0; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
        } else if (i == 1) {
            cache[1] = (seq_t)(-4);
        } else {
            seq_t term1 = (seq_t)4 * (seq_t)i * cache[i - 1];
            long long bi = (long long)(2 * (long long)i) - 7;   /* 2*i-7 may be negative */
            seq_t term2 = (seq_t)12 * (seq_t)bi * cache[i - 2];
            cache[i] = term1 - term2;
        }
    }
    return cache[n];
}
