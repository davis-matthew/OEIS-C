/*
 * A031531  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 33.
 * https://oeis.org/A031531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A031531: Numbers k such that the continued fraction for sqrt(k)
   has even period and, if the last term of the periodic part is deleted,
   the central term equals 33.
*/

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
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

static seq_t *cache = NULL;          /* stored sequence values          */
static size_t cache_cap = 0;         /* capacity of cache               */
static size_t cache_len = 0;         /* number of values stored         */
static idx_t  next_k   = 2;          /* next integer to test            */

/* Return true if D satisfies the definition of the sequence. */
static bool satisfies(idx_t D)
{
    if (D <= 1)
        return false;

    /* integer part of sqrt(D) */
    seq_t tmp = floor_sqrt((seq_t)D);
    unsigned long long a0 = (unsigned long long)tmp;

    /* skip perfect squares */
    if (a0 * a0 == D)
        return false;

    unsigned long long m = 0, d = 1, a = a0;

    /* we will collect the periodic part in a dynamic array */
    size_t cap = 64;
    size_t len = 0;
    unsigned long long *coeff = (unsigned long long*)malloc(cap * sizeof(unsigned long long));
    if (!coeff)
        return false;    /* treat as failure */

    /* Find period, store coefficients a1...aL */
    while (true) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        if (len >= cap) {
            cap <<= 1;
            unsigned long long *tmp = (unsigned long long*)realloc(coeff, cap * sizeof(unsigned long long));
            if (!tmp) { free(coeff); return false; }
            coeff = tmp;
        }
        coeff[len++] = a;

        if (a == 2 * a0)        /* last term of the period */
            break;
    }

    bool ok = false;
    if ((len & 1) == 0) {          /* even period? */
        /* after deleting last term, central coefficient is at index len/2 - 1 (0‑based) */
        size_t mid = len / 2 - 1;
        if (coeff[mid] == 33)
            ok = true;
    }

    free(coeff);
    return ok;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1 */

    /* Lazy allocation of cache. */
    if (!cache) {
        cache_cap = 128;
        cache     = mem_new(seq_t, cache_cap);
    }

    /* Generate terms until we have at least n values. */
    while (cache_len < (size_t)n) {
        idx_t D = next_k++;
        if (satisfies(D)) {
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (!tmp)   /* memory failure */
                    return 0;
                cache = tmp;
            }
            cache[cache_len++] = (seq_t)D;
        }
    }
    return cache[n - 1];
}
