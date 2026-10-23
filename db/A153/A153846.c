/*
 * A153846  Number of non-isomorphic I-graphs I(n,j,k) on 2n vertices
 *          (1<=j,k<=Floor[(n-1)/2]).
 * https://oeis.org/A153846
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1/φ(n) · Σ_{s∈U(n)} ( C( |FP_s|, 2 ) + |FP_s| + |TwoC_s|
 *            ) where U(n) is the group of units modulo n, FP_s = { x |
 *            1≤x≤⌊(n−1)/2⌋, f_s(x)=x }, TwoC_s = { x | 1≤x<y≤⌊(n−1)/2⌋,
 *            f_s(x)=y , f_s(y)=x }. f_s(x) = min( (s·x mod n), n−(s·x mod n)
 *            )
 * Range:     u8 n <= 29, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Euclidean gcd for idx_t */
static inline idx_t gcd_idx_t(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* offset is 3, lower values are irrelevant */

    /* m = ⌊(n−1)/2⌋ : maximum allowed j or k */
    idx_t m = (n - 1) / 2;

    /* pre‑allocate array for mapping f */
    idx_t *f = (idx_t*)malloc((m + 1) * sizeof(idx_t));
    if (!f) return 0;   /* allocation failure – unlikely for given bounds */

    /* compute φ(n) and iterate over all units s */
    idx_t phi = 0;
    seq_t sum_fix = 0;

    for (idx_t s = 1; s < n; ++s) {
        if (gcd_idx_t(s, n) != 1) continue;
        ++phi;

        /* compute f[x] for all 1 ≤ x ≤ m */
        for (idx_t x = 1; x <= m; ++x) {
            idx_t prod = s * x;           /* product fits in 64‑bit for n≤1e4 */
            idx_t y = prod % n;           /* 0 … n-1 */
            if (y == 0) y = n;            /* map 0 → n */
            if (y > m) y = n - y;         /* canonical value in [1,m] */
            f[x] = y;
        }

        /* count fixed points and two‑cycles */
        idx_t ff = 0;      /* |FP_s| */
        idx_t twoc = 0;    /* |TwoC_s| */

        for (idx_t x = 1; x <= m; ++x)
            if (f[x] == x) ++ff;

        for (idx_t x = 1; x <= m; ++x) {
            idx_t y = f[x];
            if (y > x && f[y] == x) ++twoc;
        }

        /* FixCount(s) = C(ff,2)+ff + twoc = ff*(ff+1)/2 + twoc */
        seq_t fix_s = ((seq_t)ff * (ff + 1)) / 2 + twoc;
        sum_fix += fix_s;
    }

    free(f);

    /* number of non‑isomorphic graphs */
    return sum_fix / phi;
}
