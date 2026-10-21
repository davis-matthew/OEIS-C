/*
 * A211921  Number of (w,x,y,z) with all terms in {1,...,n} and 2w*x<=3*y*z.
 * https://oeis.org/A211921
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y,z) ∈ {1,…,n}⁴ | 2·w·x ≤ 3·y·z}
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* maximum possible product value for pairs (y,z) and (w,x) */
    idx_t maxProd = n * n;
    size_t sz = (size_t)(maxProd + 1);

    /* frequency array for all products y*z, indexed by product value */
    uint32_t *cnt = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!cnt) return 0;                  /* allocation failed */
    memset(cnt, 0, sz * sizeof(uint32_t));

    /* count all products y*z */
    for (idx_t y = 1; y <= n; ++y) {
        uint32_t *p = cnt + y;           /* start at product y·1 */
        for (idx_t z = 1; z <= n; ++z) {
            ++*p;                        /* cnt[y*z]++ */
            p += y;                      /* next multiple of y */
        }
    }

    /* transform cnt into a suffix‑sum array:
       after this cnt[t] = number of pairs (y,z) with product ≥ t */
    for (idx_t t = maxProd; t > 0; --t) {
        cnt[t - 1] += cnt[t];
    }

    /* accumulate the answer over all (w,x) pairs */
    seq_t total = 0;
    for (idx_t w = 1; w <= n; ++w) {
        idx_t prod = 0;
        for (idx_t x = 1; x <= n; ++x) {
            prod += w;                   /* prod = w·x */
            idx_t threshold = (2 * prod + 2) / 3;  /* ceil(2·prod/3) */
            total += (seq_t)cnt[threshold];
        }
    }

    free(cnt);
    return total;
}
