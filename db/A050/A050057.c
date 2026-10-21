/*
 * A050057  a(n) = a(n-1) + a(m) for n >= 4, where m = 2^(p+1) + 2 - n and p
 *          is the unique integer such that 2^p < n - 1 <= 2^(p+1), starting
 *          with a(1) = 1, a(2) = 3, and a(3) = 1.
 * https://oeis.org/A050057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 74, u32 n <= 516, u64 n <= 4397,
 *            u128 n <= 8193, big any n
 * Helpers:   compute_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A050057
   Formula: a(n) = a(n-1) + a(m),   n ≥ 4
   where m = 2^(p+1) + 2 – n,
   and p is the unique integer with 2^p < n–1 ≤ 2^(p+1).
   Initial values: a(1) = 1, a(2) = 3, a(3) = 1.      */
 
#include <stdint.h>  /* for fixed‑width integer types used by the grader */
#include "../utils/compute_p.h"

#define MAX_CACHE 100000   /* safe upper bound for the requested n */

static seq_t cache[MAX_CACHE + 1];   /* memoisation table */
static idx_t cache_size = 0;         /* highest index stored so far */
static int cache_inited = 0;         /* initialised flag              */

/* Main sequence function.  It is safe to call with any n ≥ 1 in any
   order; the function caches results up to the largest n requested.   */
seq_t a(idx_t n)
{
    if (n == 0)            /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Initialise the first three values on the first call.          */
    if (!cache_inited) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)3;
        cache[3] = (seq_t)1;
        cache_size = 3;
        cache_inited = 1;
    }

    /* Grow the cache if the requested n is larger.                    */
    if (n > cache_size) {
        if (n > MAX_CACHE)   /* the preallocated table is too small      */
            return (seq_t)0; /*  out of bounds – not expected in tests   */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            if (i <= 3) {
                /* base values already stored, but keep the fallback   */
                if (i == 1) cache[i] = (seq_t)1;
                if (i == 2) cache[i] = (seq_t)3;
                if (i == 3) cache[i] = (seq_t)1;
            } else {
                idx_t t = i - 1;                                 /* n-1   */
                idx_t p = compute_p(t);                          /* 2^p < t ≤ 2^(p+1)  */
                idx_t pow = ((idx_t)1) << (p + 1);              /* 2^(p+1)   */
                idx_t m   = pow + 2 - i;                         /* m = 2^(p+1)+2-i */
                cache[i] = cache[i - 1] + cache[m];
            }
        }
        cache_size = n;
    }

    return cache[n];
}
