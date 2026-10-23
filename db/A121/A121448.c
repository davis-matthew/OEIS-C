/*
 * A121448  Triangle read by rows: T(n,k) is the number of binary trees with n
 *          edges and having k vertices of outdegree 1 (n>=0, k>=0). A binary
 *          tree is a rooted tree in which each vertex has at most two
 *          children and each child of a vertex is designated as its left or
 *          right child.
 * https://oeis.org/A121448
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   binomial, offset_idx, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A121448
 * Formula: a(n) is the (r,k)th entry of the triangle
 *          T(r,k) = 2^k * C(r+1,k) * C(r+1-k,(r-k)/2) / (r+1),
 *           where r is the largest integer with r(r+1)/2 ≤ n,
 *           k = n – r(r+1)/2.  If r-k is odd then T(r,k)=0.
 */
#include "../utils/binomial.h" /* binomial() */
#include "../utils/seq_mul.h"  /* seq_mul() */
#include "../utils/offset_idx.h"

seq_t a(idx_t n)
{
    /* Find r: offset(r) ≤ n < offset(r+1) */
    idx_t low  = 0;
    idx_t high = n;                /* safe upper bound */

    while (low < high)
    {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 prod = ( (unsigned __int128)mid * (mid + 1) ) / 2;
        if (prod <= n)
            low = mid;
        else
            high = mid - 1;
    }

    idx_t r        = low;
    idx_t offset_r = offset_idx(r);
    idx_t k        = n - offset_r;

    /* parity check */
    if ( ((r - k) & 1) != 0 )
        return 0;

    /* Compute 2^k */
    seq_t val = 1;
    for (idx_t i = 0; i < k; ++i)
        val = seq_mul(val, (seq_t)2);

    /* Multiply by binomial(r+1,k) */
    val = seq_mul(val, binomial(r + 1, k));

    /* Multiply by binomial(r+1-k,(r-k)/2) */
    val = seq_mul(val, binomial(r + 1 - k, (r - k) / 2));

    /* Divide by (r+1) */
    val = val / (r + 1);

    return val;
}
