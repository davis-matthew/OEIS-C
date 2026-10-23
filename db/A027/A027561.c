/*
 * A027561  Number of 4-unbalanced strings of length n (=2^n-A027559(n)).
 * https://oeis.org/A027561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   seq_realloc_proxy
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2*Fib(n+3) + 2^n + (5*(-1)^n - 19)*3^[floor(n/2)-1].
 * Recurrence (for n ≥ 6):
 *     a(n) = 3*a(n-1) + 2*a(n-2) - 11*a(n-3) + 3*a(n-4) + 6*a(n-5),
 * with initial values
 *     a(0) = a(1) = a(2) = a(3) = a(4) = 0,  a(5) = 2.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_realloc_proxy.h"

seq_t a(idx_t n)
{
    /* Static memoisation table */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of entries stored */

    /* Ensure capacity: at least n+1 entries */
    size_t need_len = (size_t)n + 1;
    if (need_len > cache_len) {
        seq_t *tmp = seq_realloc_proxy(cache, cache_len, need_len);
        if (!tmp) return 0;                /* allocation failure; unlikely for given limits */
        cache = tmp;
        /* Initialise new elements to zero */
        for (size_t i = cache_len; i < need_len; ++i)
            cache[i] = 0;
        cache_len = need_len;
    }

    /* Initialise known base values if not already set */
    if (cache_len > 5 && cache[5] == 0) {   /* assume zero indicates uninitialised for this slot */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 0;
        cache[4] = 0;
        cache[5] = 2;
    }

    /* Compute missing values using the recurrence */
    if (n >= 6) {
        idx_t start = (idx_t)5;          /* last index already correct is 5 */
        for (idx_t i = start + 1; i <= n; ++i) {  /* i starts at 6 */
            seq_t t1 = 3 * cache[i-1];   /* 3*a(i-1) */
            seq_t t2 = 2 * cache[i-2];   /* 2*a(i-2) */
            seq_t t3 = 3 * cache[i-4];   /* 3*a(i-4) */
            seq_t t4 = 6 * cache[i-5];   /* 6*a(i-5) */
            seq_t t5 = 11 * cache[i-3];  /* 11*a(i-3) */
            cache[i] = t1 + t2 + t3 + t4 - t5;
        }
    }

    return cache[n];
}
