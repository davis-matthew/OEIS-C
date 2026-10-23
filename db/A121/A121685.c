/*
 * A121685  Triangle read by rows: T(n,k) is the number of binary trees having
 *          n edges and k branches (1<=k<=n). A binary tree is a rooted tree
 *          in which each vertex has at most two children and each child of a
 *          vertex is designated as its left or right child.
 * https://oeis.org/A121685
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   T(n,k) = 2^(n-k)*C_{k/2}*binomial(n-1,k-1)   if k even
   T(n,k) = 2^(n-k+1)*C_{(k-1)/2}*binomial(n-1,k-1)  if k odd

   a(n) is the n‑th element of the triangle read by rows, i.e.  
   a(n) = T(r,c) where r is the row containing position n and c is  
   the column index in that row. */
#include <math.h>

static seq_t mul_pow2(idx_t exp)
{
    seq_t r = 1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * 2;
    return r;
}

static seq_t binomial_mul(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (n - k + i);
        res = res / i;   /* exact division */
    }
    return res;
}

static seq_t catalan(idx_t m)
{
    /* C_m = binomial(2m, m)/(m+1) */
    seq_t b = binomial_mul(2 * m, m);
    return b / (m + 1);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find row r such that r(r+1)/2 >= n > (r-1)r/2 */
    double d = sqrt(8.0 * (double)n + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    if (r * (r + 1) / 2 < n) ++r;

    /* Index of first element of row r */
    idx_t first = (r - 1) * r / 2 + 1;
    idx_t k = n - first + 1;   /* column (1‑based) */

    /* Compute the triangle entry */
    seq_t pow_factor;
    seq_t cat_factor;

    if ((k & 1) == 0)       /* k even */
    {
        pow_factor = mul_pow2(r - k);
        cat_factor = catalan(k / 2);
    }
    else                    /* k odd */
    {
        pow_factor = mul_pow2(r - k + 1);
        cat_factor = catalan((k - 1) / 2);
    }

    seq_t bin_factor = binomial_mul(r - 1, k - 1);

    return pow_factor * cat_factor * bin_factor;
}
