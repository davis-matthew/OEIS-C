/*
 * A132391  Numbers whose square starts with 4 identical digits.
 * https://oeis.org/A132391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 32, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 50 terms through n = 9614 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132391: numbers whose square starts with four identical digits
   The sequence is generated on demand by scanning the ranges of squares
   that begin with dddd for d = 1 … 9.  The algorithm keeps state between
   calls so that arbitrary order of requests is supported.  All arithmetic
   is performed with the seq_t type supplied by the build.  No external
   libraries are required beyond the provided utils.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache      = NULL;      /* array of terms */
    static size_t cacheSize  = 0;         /* allocated capacity */
    static size_t cacheCount = 0;         /* number of terms stored */

    /* state for incremental generation */
    static idx_t D   = 4;                 /* current number of digits of squares */
    static int  d    = 1;                 /* current leading digit (1..9) */
    static seq_t block_start = 0;         /* first m in current block */
    static seq_t block_end   = 0;         /* last  m in current block  */
    static seq_t next_m      = 0;         /* next m to emit */

    /* initialise first block if necessary */
    if (next_m == 0) {
        /* Compute first non‑empty block */
        while (true) {
            seq_t pow10Exp = pow10(D - 4);  /* 10^(D-4) */
            seq_t L = (seq_t)(1111 * d) * pow10Exp;
            seq_t U = (seq_t)(1111 * d + 1) * pow10Exp - 1;

            seq_t f_start = floor_sqrt(L);
            seq_t start   = f_start;
            if (f_start * f_start < L)
                start = f_start + 1;

            seq_t end = floor_sqrt(U);

            block_start = start;
            block_end   = end;
            next_m      = block_start;

            if (block_start <= block_end) break;   /* found a non‑empty block */

            /* advance to next block */
            d++;
            if (d > 9) { d = 1; D++; }
        }
    }

    /* generate terms until we have at least n */
    while (cacheCount < n) {
        /* ensure we are still within the current block; otherwise advance */
        if (next_m > block_end) {
            /* move to the next d, and if necessary to the next D */
            d++;
            if (d > 9) { d = 1; D++; }

            /* compute the next block */
            seq_t pow10Exp = pow10(D - 4);
            seq_t L = (seq_t)(1111 * d) * pow10Exp;
            seq_t U = (seq_t)(1111 * d + 1) * pow10Exp - 1;

            seq_t f_start = floor_sqrt(L);
            seq_t start   = f_start;
            if (f_start * f_start < L)
                start = f_start + 1;

            seq_t end = floor_sqrt(U);

            block_start = start;
            block_end   = end;
            next_m      = block_start;
            continue;   /* retry with new block */
        }

        /* allocate cache if necessary */
        if (cacheCount >= cacheSize) {
            size_t newSize = cacheSize ? cacheSize * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, newSize);
            cache = tmp;
            cacheSize = newSize;
        }

        /* store current term and advance to next */
        cache[cacheCount++] = next_m;
        ++next_m;
    }

    /* now cache is guaranteed to have at least n terms */
    return cache[n - 1];
}
