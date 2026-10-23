/*
 * A170856  a(0) = 0, a(1) = 1; a(2^i + j) = 2a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(0)=0, a(1)=1; for n>1 write n=2^i + j with 0≤j<2^i, then
 *            a(n)=2*a(j)+3*a(j+1)
 * Range:     u8 n <= 27, u16 n <= 250, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static inline idx_t floor_log2_idx(idx_t v) {
    /* v>0 */
    return 63 - __builtin_clzll(v);
}

seq_t a(idx_t n) {
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static bool *used  = NULL;
    static idx_t cap   = 0;
    static idx_t sz    = 0;   /* number of computed entries */

    /* Grow cache if needed */
    if (n + 1 > cap) {
        size_t new_cap = cap ? cap << 1 : 4;
        while (new_cap < n + 1) new_cap <<= 1;
        seq_t *nc = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        bool   *nu = (bool   *)realloc(used,  new_cap * sizeof(bool   ));
        if (!nc || !nu) abort();              /* out of memory */
        cache = nc; used = nu; cap = new_cap;
    }

    if (n < sz && used[n]) return cache[n];   /* already computed */

    seq_t result;
    if (n == 0)
        result = (seq_t)0;
    else if (n == 1)
        result = (seq_t)1;
    else {
        idx_t i = floor_log2_idx(n);
        idx_t p = (idx_t)1 << i;          /* 2^i <= n */
        idx_t j = n - p;                   /* 0 ≤ j < 2^i */
        seq_t aj  = a(j);
        seq_t aj1 = a(j + 1);
        result = (seq_t)2 * aj + (seq_t)3 * aj1;
    }

    /* Store computed value */
    if (n >= sz) sz = n + 1;
    used[n] = true;
    cache[n] = result;
    return result;
}
