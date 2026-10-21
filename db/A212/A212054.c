/*
 * A212054  Number of (w,x,y,z) with all terms in {1,...,n} and w*x>y*z+1.
 * https://oeis.org/A212054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = #{(w,x,y,z) : 1≤w,x,y,z≤n, w*x > y*z + 1}
 * Computation: count multiplicities of all products p = w*x, 
 * then accumulate pairs p > q + 1 using prefix sums.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* 0‑based offset: a(0) = 0  */
    if (n == 0) return 0;

    /* Maximum product is n^2 */
    idx_t maxprod = n * n;

    /* Allocate an array of multiplicities for products 0 … maxprod.
     * idx_t may be 64‑bit, but size_t is also 64‑bit on the target platform. */
    uint16_t *cnt = (uint16_t *)malloc((size_t)(maxprod + 1) * sizeof(uint16_t));
    if (!cnt) return 0;                 /* Allocation failed → return 0 */

    /* Zero initialize the array; cnt[0] stays 0 */
    memset(cnt, 0, (size_t)(maxprod + 1) * sizeof(uint16_t));

    /* Build multiplicities: count pairs (w,x) giving each product p = w*x */
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            /* index = w * x; this fits in idx_t for the given limits */
            idx_t prod = w * x;
            cnt[(size_t)prod]++;        /* count[p] += 1 */
        }
    }

    /* Now accumulate the answer using prefix sums.
     * pref  = Σ_{q <= current-1} cnt[q]
     * For product p we need Σ_{q <= p-2} cnt[q] = pref - cnt[p-1]  (cnt[0] = 0) */
    seq_t pref = 0;   /* Σ_{q < p-1} cnt[q]  (updated after each iteration) */
    seq_t ans  = 0;

    for (idx_t p = 1; p <= maxprod; ++p) {
        seq_t cntp         = (seq_t)cnt[(size_t)p];
        seq_t cnt_before   = (seq_t)cnt[(size_t)(p - 1)];   /* cnt[0] is safe */
        seq_t pref_before  = pref - cnt_before;            /* Σ_{q ≤ p-2} cnt[q] */
        ans += cntp * pref_before;
        pref += cntp;      /* ready for next p+1 */
    }

    free(cnt);
    return ans;
}
