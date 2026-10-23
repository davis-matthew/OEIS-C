/*
 * A039757  Triangle of coefficients in expansion of
 *          (x-1)*(x-3)*(x-5)*...*(x-(2*n-1)).
 * https://oeis.org/A039757
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign, easy, nice
 * Formula:   a(n) = coefficient of x^c in the product * ∏_{i=1}^{r} (x – (2·i
 *            – 1)), * where r and c are determined by the triangular indexing
 *            * n = r·(r+1)/2 + c (0 ≤ c ≤ r). * This produces the flattened
 *            triangle of coefficients of * (x–1)(x–3)(x–5)…(x–(2n–1))
 * Range:     i8 n <= 10, i16 n <= 27, i32 n <= 65, i64 n <= 153,
 *            i128 n <= 434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

static inline idx_t isqrt_u64(idx_t n)
{
    /* floor(sqrt(n)) without overflow */
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) {          /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= n / mid)      /* mid*mid <= n  without overflow */
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* 1. find row r and column c by triangular indexing */
    idx_t d      = (idx_t)1 + (idx_t)8 * n;   /* 1+8n  */
    idx_t sqrt_d = isqrt_u64(d);
    idx_t r      = (sqrt_d - 1) >> 1;          /* floor((-1+sqrt_d)/2) */

    /* adjust in case of rounding errors */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n)          --r;

    idx_t start = r * (r + 1) / 2;
    idx_t c     = n - start;              /* 0 ≤ c ≤ r */

    /* 2. compute triangle up to row r */

    /* maximum n needed in this problem is about 10000,
       so r ≤ 141.  We use a modest static buffer. */
    #define MAX_R 2000
    seq_t prev[MAX_R + 1];
    seq_t curr[MAX_R + 1];
    seq_t odd = 1;                         /* odd double factorial */

    for (idx_t i = 0; i <= r; ++i) {
        if (i == 0) {
            curr[0] = 1;                   /* (-1)^0 * 1!! = 1 */
        } else {
            odd *= (seq_t)(2 * i - 1);     /* update odd double factorial */
            curr[0] = ( ((i & 1) == 0) ? odd : -odd );
        }

        /* interior terms */
        for (idx_t j = 1; j < i; ++j) {
            curr[j] = prev[j - 1] - ((seq_t)(2 * i - 1)) * prev[j];
        }

        /* last term */
        if (i > 0)
            curr[i] = 1;

        /* copy curr → prev for next iteration */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = curr[j];
    }

    return prev[c];
}
