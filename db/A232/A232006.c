/*
 * A232006  Triangular array read by rows: T(n, k) is the number of simple
 *          labeled graphs on vertex set {1,2,...,n} with exactly k components
 *          (all of which are trees) such that the labels {1,2,...,k} are all
 *          in distinct components (trees), n >= 0, 0 <= k <= n.
 * https://oeis.org/A232006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2 and T(r,0) = 0 (r>0), T(r,r) = 1 T(r,k) = k *
 *            r^(r-k-1) (1 <= k <= r-1, r>0)
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 78, u64 n <= 171,
 *            u128 n <= 435, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

/* compute the row index r for a given triangular index n */
static inline idx_t triangular_row(idx_t n)
{
    idx_t low = 0, high = 1;
    /* find an upper bound */
    while ((high * (high + 1)) / 2 <= n) high <<= 1;
    low = high >> 1;
    /* binary search */
    while (high - low > 1) {
        idx_t mid = low + (high - low) / 2;
        if ((mid * (mid + 1)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Special case: the very first element (n==0) */
    if (n == 0) return (seq_t)1;

    /* Determine (r,k) from the row-major index */
    idx_t r = triangular_row(n);
    idx_t start = (r * (r + 1)) / 2;
    idx_t k = n - start;       /* 0 <= k <= r */

    /* Compute T(r,k) according to the definition */
    if (k == 0) return (seq_t)0;           /* n > 0 here */
    if (k == r) return (seq_t)1;           /* r components, all singletons */

    /* For 1 <= k <= r-1 :  T(r,k) = k * r^(r-k-1) */
    seq_t pow_val = big_pow((seq_t)r, (seq_t)(r - k - 1));
    seq_t result  = pow_val * (seq_t)k;
    return result;
}
