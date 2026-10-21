/*
 * A211053  Number of 2 X 2 matrices having all terms in {1,...,n} and
 *          determinant n.
 * https://oeis.org/A211053
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ (a,b,c,d)∈[1,n]^4 | ad - bc = n }
 * Range:     u8 n <= 11, u16 n <= 143, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of 2×2 matrices with entries in {1,…,n} and determinant = n. */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* maximum product of a and d (or b and c) is n*n */
    size_t maxProd = (size_t)n * (size_t)n;
    /* allocate an array of unsigned 16‑bit counters (fits all counts) */
    uint16_t *cnt = (uint16_t *)malloc((maxProd + 1) * sizeof(uint16_t));
    if (!cnt) return (seq_t)0;            /* out of memory → return 0 */

    /* initialize to zero */
    for (size_t i = 0; i <= maxProd; ++i) cnt[i] = 0;

    /* count all ordered pairs (a,d) with product k (1 ≤ a,d ≤ n) */
    for (idx_t a = 1; a <= n; ++a) {
        for (idx_t d = 1; d <= n; ++d) {
            size_t prod = (size_t)a * (size_t)d;
            cnt[prod]++;           /* uint16_t holds the count */
        }
    }

    seq_t ans = 0;

    /* for each ordered pair (b,c) compute p = n + b*c;  add cnt[p] if p≤maxProd */
    for (idx_t b = 1; b <= n; ++b) {
        for (idx_t c = 1; c <= n; ++c) {
            size_t prod_bc = (size_t)b * (size_t)c;       /* ≤ maxProd */
            size_t p = prod_bc + (size_t)n;              /* desired ad */
            if (p <= maxProd) {
                ans += (seq_t)cnt[p];
            }
        }
    }

    free(cnt);
    return ans;
}
