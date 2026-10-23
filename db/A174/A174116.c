/*
 * A174116  Triangle T(n, k) = (n/2)*binomial(n-1, k-1)*binomial(n-1, k) with
 *          T(n, 0) = T(n, n) = 1, read by rows.
 * https://oeis.org/A174116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = the nth element of the triangle T(n,k) defined by *
 *            T(n,k) = (n/2)*binomial(n-1,k-1)*binomial(n-1,k), * where the
 *            triangle is read row‑by‑row starting at n=0. * r =
 *            floor((sqrt(8*n+1)-1)/2) gives the row of n, * k = n - r*(r+1)/2
 *            gives the column. * For boundary values k=0 or k=r, T=1
 * Range:     u8 n <= 23, u16 n <= 59, u32 n <= 179, u64 n <= 610,
 *            u128 n <= 2242, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include <stdint.h>

/* Integer square root for idx_t: floor(sqrt(x)).  No <math.h> is used. */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = (idx_t)1 << 32;  /* 2^32 > sqrt(2^64) */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t mid2 = mid * mid;
        if (mid2 == x)
            return mid;
        if (mid2 < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = (isqrt_u64(8 * n + 1) - 1) >> 1;   /* (sqrt(8n+1)-1)/2 */

    /* Start index of this row. */
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;        /* column within the row */

    /* Boundary values are 1. */
    if (k == 0 || k == r)
        return (seq_t)1;

    /* Compute binomial terms: C(r-1, k-1) and C(r-1, k). */
    seq_t b1 = binomial(r - 1, k - 1);
    seq_t b2 = binomial(r - 1, k);

    /* Multiply while avoiding overflow by dividing first. */
    seq_t prod = b1 * b2;
    seq_t result;
    if ((r & 1) == 0) {          /* r even: r/2 * prod */
        result = ((seq_t)(r / 2)) * prod;
    } else {                     /* r odd: prod/2 * r  (prod is even) */
        result = ((seq_t)r) * (prod / 2);
    }
    return result;
}
