/*
 * A211918  Number of (w,x,y,z) with all terms in {1,...,n} and w*x>3*y*z.
 * https://oeis.org/A211918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(w,x,y,z) | 1≤w,x,y,z≤n and w*x > 3*y*z}
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: precompute, for each t, the number of ordered pairs (y,z) with
   product ≤ t (t up to ⌊(n²–1)/3⌋).  Then for each ordered pair (w,x) add
   the corresponding count.  Complexity: O(n²) + O((n²/3)) time and
   O(n²/3) auxiliary space. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/* External types defined by the build */
typedef seq_t seq_t;   /* unsigned integer of any size */
typedef idx_t idx_t;   /* unsigned 64‑bit */

seq_t a(idx_t n)
{
    /* trivial cases */
    if (n < 2) return 0;

    /* maximum t needed: floor((n²‑1)/3) */
    idx_t N = (n * n - (idx_t)1) / (idx_t)3;
    size_t sz = (size_t)N + 1;          /* array size */

    /* count of ordered pairs (y,z) with product == p  (array f[p]) */
    uint32_t *f = (uint32_t *)calloc(sz, sizeof(uint32_t));
    if (!f) return 0;                   /* allocation failure → default 0 */

    /* populate f[p] = #{(y,z) | y*z == p} */
    for (idx_t y = 1; y <= n; ++y) {
        idx_t maxz = N / y;
        if (maxz > n) maxz = n;
        for (idx_t z = 1; z <= maxz; ++z) {
            size_t p = (size_t)(y * z);
            f[p]++;                      /* unordered multiplicity */
        }
    }

    /* transform f to prefix sums: f[t] becomes #{(y,z) | y*z ≤ t} */
    for (size_t t = 1; t < sz; ++t) {
        f[t] += f[t - 1];
    }

    /* compute the answer by summing over all ordered pairs (w,x) */
    seq_t ans = 0;
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            idx_t prod = w * x;
            idx_t T = (prod - (idx_t)1) / (idx_t)3;  /* floor((w*x–1)/3) */
            ans += (seq_t)f[(size_t)T];
        }
    }

    free(f);
    return ans;
}
