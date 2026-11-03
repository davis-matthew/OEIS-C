/*
 * A097183  Main diagonal of triangle A097181, in which the n-th row
 *          polynomial R_n(y) is formed from the initial (n+1) terms of g.f.
 *          A097182(y)^(n+1), where R_n(1/2) = 8^n for all n>=0.
 * https://oeis.org/A097183
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = ((16*n-2)/n) * a(n-1) for n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 197 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the recurrence derived from the generating function 1/(1-16x)^(7/8) */

#include <stdlib.h>
#include "../utils/mem.h"

/* Helper that multiplies a by (16*n-2) and divides by n.
   For 64‑bit integral seq_t types a 128‑bit intermediate is used;
   for arbitrary‑precision seq_t types the native operators are used. */
static inline seq_t compute_next(seq_t prev, idx_t n)
{
    unsigned long long mult = 16ULL * n - 2ULL;            /* 16*n-2  (fits 64‑bit for used n) */

#if defined(__SIZEOF_INT128__)                          /* 128‑bit intermediate available */
    /* When seq_t is an unsigned integer of ≤8 bytes we can use 128‑bit math. */
    if (sizeof(seq_t) <= 8) {
        __int128 prod = (__int128)( (uint64_t)prev ) * mult;
        return (seq_t)(prod / n);
    }
    /* For larger seq_t (arbitrary precision) fall back on the native operators. */
#endif
    /* Generic path for arbitrary‑precision types or when 128‑bit not supported. */
    return (prev * (seq_t)mult) / (seq_t)n;
}

/* Memoised implementation of A097183.  Calls may arrive in any order.  */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0] = a(0) = 1 */
    static idx_t cache_size = 0;     /* number of elements stored */

    if (cache_size == 0) {           /* first call – initialise cache */
        cache = mem_new(seq_t, 1);
        if (!cache) return (seq_t)0; /* allocation failed – return 0 */
        cache[0] = (seq_t)1;         /* a(0) */
        cache_size = 1;
    }

    /* Grow the cache up to the requested index. */
    while (cache_size <= n) {
        idx_t next_idx = cache_size;                 /* index to compute */
        seq_t prev = cache[next_idx - 1];            /* a(next_idx-1) */
        seq_t next_val = compute_next(prev, next_idx);
        seq_t *tmp = mem_resize(seq_t, cache, (next_idx + 1));
        if (!tmp) return (seq_t)0;                    /* allocation failed */
        cache = tmp;
        cache[next_idx] = next_val;
        cache_size++;
    }

    return cache[n];
}
