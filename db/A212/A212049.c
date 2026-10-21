/*
 * A212049  Number of (w,x,y,z) with all terms in {1,...,n} and w*x=5*y*z.
 * https://oeis.org/A212049
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y,z) ∈ {1,…,n}^4 : w*x = 5*y*z}
 * Range:     u8 n <= 14, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* For n = 0 the answer is 0 – loops below will skip automatically. */
    idx_t N = n;

    /* The largest product that can appear on the right‑hand side after
     * multiplying by 5 is 5·(N·N).  Therefore the largest value that can
     * occur in the array `cnt` (y*z values) is ⌊N*N/5⌋.  We allocate one
     * more slot for the index 0 which will never be used.           */
    idx_t max_prod_yz = (N * N) / 5;
    idx_t limit = max_prod_yz + 1;                /* indices 0 … limit-1 */

    /* Use 32‑bit counters – the number of ordered pairs (y,z) with a given
     * product is bounded by the divisor function τ(t), which is < 1000
     * for t ≤ 10⁸.  32 bits are more than enough and keep the array small. */
    uint32_t *cnt = (uint32_t *)calloc((size_t)limit, sizeof(uint32_t));
    if (!cnt) return 0;   /* allocation failure – return 0 as safe fallback */

    /* Count all ordered pairs (y,z) with product ≤ max_prod_yz. */
    for (idx_t y = 1; y <= N; ++y) {
        for (idx_t z = 1; z <= N; ++z) {
            idx_t prod = y * z;
            if (prod >= limit) break;   /* z is increasing – can stop early */
            ++cnt[prod];
        }
    }

    seq_t result = 0;

    /* For each pair (w,x) compute w*x.  If it is divisible by 5,
     * add the number of (y,z) pairs that realise the required product. */
    for (idx_t w = 1; w <= N; ++w) {
        for (idx_t x = 1; x <= N; ++x) {
            idx_t prod_wx = w * x;
            if (prod_wx % 5 != 0) continue;
            idx_t t = prod_wx / 5;
            if (t < limit) {
                result += (seq_t)cnt[t];
            }
        }
    }

    free(cnt);
    return result;
}
