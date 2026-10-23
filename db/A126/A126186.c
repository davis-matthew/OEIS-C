/*
 * A126186  Triangle read by rows: T(n,k) is number of hex trees with n edges
 *          and level of first leaf (in the preorder traversal) equal to k (1
 *          <= k <= n).
 * https://oeis.org/A126186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r = row number (starting at 1), k = column
 *            number and T(r,k) is given by the OEIS entry A126186: T(r,r) =
 *            3^r T(r,k) (k<r) = [k/(r-k)] * Σ_{j=ceil((r-2k)/2)}^{r-k}
 *            3^(2k+2j-r) * binomial(r-k,j) * binomial(k-1+j, r-k-1-j)
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial, mul_small, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* binomial(idx_t n, idx_t k) -> seq_t   */
#include "../utils/pow3.h"        /* pow3(idx_t exp) -> seq_t             */
#include "../utils/mul_small.h"   /* mul_small(seq_t x, unsigned int m) -> seq_t */

static seq_t T_row_col(idx_t r, idx_t k)
{
    if (k == r)                    /* last column */
        return pow3(r);

    /* general case (k < r) */
    idx_t n = r;                       /* rename for clarity */
    long long diff = (long long)n - 2LL * (long long)k;
    idx_t j_start = (diff <= 0) ? 0 : (idx_t)((diff + 1) / 2);
    idx_t j_end   = n - k;

    seq_t sum = 0;
    for (idx_t j = j_start; j <= j_end; ++j)
    {
        seq_t b1 = binomial(n - k, j);
        if (!b1) continue;               /* skip zero terms   */
        seq_t b2 = binomial(k - 1 + j,
                            n - k - 1 - j);
        if (!b2) continue;

        idx_t exp = 2 * k + 2 * j - n;
        seq_t term = pow3(exp);
        term = term * b1;                /* multiplication works on seq_t   */
        term = term * b2;
        sum += term;
    }

    seq_t prod   = mul_small(sum, (unsigned int)k);
    return prod / (seq_t)(n - k);        /* final division         */
}

static idx_t row_from_index(idx_t n)
{
    /* binary search for minimal r with r*(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for offset 1, but safe */

    idx_t r = row_from_index(n);
    idx_t k = n - (r - 1) * r / 2;    /* column within the row (1‑based) */

    return T_row_col(r, k);
}
