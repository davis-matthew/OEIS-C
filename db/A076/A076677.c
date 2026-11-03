/*
 * A076677  a(0)=a(1)=1 a(n)=a(n-1)+floor(sqrt(a(n-2))).
 * https://oeis.org/A076677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=1; a(n)=a(n-1)+floor(sqrt(a(n-2)))
 * Range:     u8 n <= 21, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 59 terms through n = 58 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

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
    static seq_t *val = NULL;
    static idx_t cap = 0;
    static idx_t last = 1;   /* last index that has been computed */

    /* Initial allocation for indices 0 and 1 */
    if (!val) {
        cap = 2;
#if defined(__cplusplus)
        val = new seq_t[cap];
#else
        val = (seq_t*)malloc(cap * sizeof(seq_t));
#endif
        if (!val) return (seq_t)0;          /* allocation failure */
        val[0] = (seq_t)1;
        val[1] = (seq_t)1;
    }

    /* Grow the cache if necessary */
    if (n >= cap) {
        idx_t new_cap = (n + 1) * 2;
#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        for (idx_t i = 0; i < cap; ++i) tmp[i] = val[i];
        delete[] val;
        val = tmp;
#else
        seq_t *tmp = (seq_t*)realloc(val, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failure */
        val = tmp;
#endif
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t s = floor_sqrt(val[i - 2]); /* floor(sqrt(a(i-2))) */
        val[i] = val[i - 1] + s;
    }

    if (last < n) last = n;
    return val[n];
}
