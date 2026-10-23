/*
 * A049888  a(n) = a(1) + a(2) + ... + a(n-1) - a(m) for n >= 4, where m = 2*n
 *          - 3 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 2.
 * https://oeis.org/A049888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A049888
   a(1)=1, a(2)=1, a(3)=2.
   For n >= 4:
       let p = floor_log2(n-2)      // p is the unique integer with 2^p < n-1 <= 2^(p+1)
       let m = 2*n - 3 - 2^(p+1)
       a(n) = (a(1)+a(2)+...+a(n-1)) - a(m)
*/

#include <stdlib.h>

static inline idx_t floor_log2_idx(idx_t x) {
    /* x > 0 */
    idx_t r = 0;
    while (x >>= 1) r++;
    return r;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;          // undefined for n < 1, guard

    static seq_t *vals = NULL;    // a(n)
    static seq_t *cum  = NULL;    // cumulative sums cum[n] = a(1)+...+a(n)
    static size_t cap     = 0;    // allocated array size
    static idx_t computed = 0;    // highest index already computed

    /* allocate / grow arrays */
    if (n + 1 > (idx_t)cap) {
        size_t new_cap = ((size_t)n + 1) * 2;
        if (vals == NULL) {
            vals = (seq_t*)malloc(new_cap * sizeof(seq_t));
            cum  = (seq_t*)malloc(new_cap * sizeof(seq_t));
            if (!vals || !cum) abort();
            cum[0] = 0;
        } else {
            vals = (seq_t*)realloc(vals, new_cap * sizeof(seq_t));
            cum  = (seq_t*)realloc(cum,  new_cap * sizeof(seq_t));
            if (!vals || !cum) abort();
        }
        cap = new_cap;
    }

    /* compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k == 1) {
            vals[1] = 1;
            cum[1]  = 1;
        } else if (k == 2) {
            vals[2] = 1;
            cum[2]  = 2;
        } else if (k == 3) {
            vals[3] = 2;
            cum[3]  = 4;
        } else {
            idx_t p     = floor_log2_idx(k - 2);
            idx_t shift = ((idx_t)1) << (p + 1);          // 2^(p+1)
            idx_t m     = 2 * k - 3 - shift;              // m < k
            seq_t a_m   = vals[m];
            seq_t sum_prev = cum[k - 1];
            seq_t a_k     = sum_prev - a_m;
            vals[k] = a_k;
            cum[k]  = sum_prev + a_k;
        }
    }
    computed = n;

    return vals[n];
}
