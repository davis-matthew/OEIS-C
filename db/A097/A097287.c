/*
 * A097287  Rectangular array T by antidiagonals: T(n,k) = rank of n where
 *          (n,k) occurs in A097285, for distinct n,k and T(n,n)=0 for all n.
 * https://oeis.org/A097287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is obtained from n by * reading the
 *            infinite matrix by antidiagonals, * and T(n,k) = 0 if n==k, *
 *            T(n,k) = (k-1)(k-2)+2n-1 if n<k, * T(n,k) = n(n-1) if n>k and
 *            k==1, * T(n,k) = (n-1)(n-2)+2k-2 if n>k and k>1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
 
seq_t a(idx_t n)
{
    /* find the diagonal sum s such that (s-1)*s/2 >= n */
    idx_t lo = 2, hi = 2;
    while (((unsigned __int128)hi * (hi - 1) / 2) < n)
        hi <<= 1;                      /* increase upper bound */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (((unsigned __int128)mid * (mid - 1) / 2) >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    idx_t s = lo;                       /* diagonal sum */
    /* cumulative terms before diagonal s */
    idx_t prev = (s - 2) * (s - 1) / 2; /* safe in 128‑bit above */
    idx_t offset = n - prev - 1;        /* 0‑based position on diagonal s */
    idx_t i = offset + 1;
    idx_t j = s - 1 - offset;
 
    if (i == j)
        return 0;
    if (i < j) {
        seq_t part1 = (seq_t)(j - 1) * (j - 2);
        return part1 + (seq_t)(2 * i - 1);
    } else { /* i > j */
        if (j == 1)
            return (seq_t)i * (i - 1);
        seq_t part1 = (seq_t)(i - 1) * (i - 2);
        return part1 + (seq_t)(2 * j - 2);
    }
}
