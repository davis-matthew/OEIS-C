/*
 * A070895  Triangle read by rows where T(n+1,k)=T(n,k)+n*T(n-1,k) starting
 *          with T(n,n)=1 and T(n,k)=0 if n<k.
 * https://oeis.org/A070895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   COMPUTATION: T(r,k) from the triangle recurrence
   T(n+1,k) = T(n,k) + n*T(n-1,k)    (for n>=1, 0≤k≤n)
   T(n,n) = 1,  T(n,k) = 0 if k>n.
   a(n) is the triangle read by rows:  a(n) = T(r,k)
   where r satisfies  r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,
   and  k = n - r*(r+1)/2.

   This implementation evaluates T(r,k) by iterating the recurrence
   along the column k.  The number of steps is r-k ≤ r ≤ √(2n),
   so each call needs only O(√n) time and O(1) extra memory.
*/

#include <stdint.h>

/* Find the row number r for which r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
static idx_t find_row(idx_t n)
{
    /* r is at most about sqrt(2n).  2^32 > sqrt(2*2^64) ≈ 4.3e9. */
    idx_t lo = 0;
    idx_t hi = (idx_t)1 << 32;          /* exclusive upper bound */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute T(r,k) for 0 ≤ k ≤ r.  Uses the recurrence
   S[m] = S[m-1] + (k+m-1)*S[m-2],  with S[0]=S[1]=1. */
static seq_t compute_T(idx_t r, idx_t k)
{
    if (k > r)           return 0;          /* safety */
    if (k == r)          return 1;
    if (k == r - 1)      return 1;

    seq_t prev2 = 1;                         /* S[0] */
    seq_t prev1 = 1;                         /* S[1] */
    idx_t mmax = r - k;                      /* number of steps needed */

    for (idx_t m = 2; m <= mmax; ++m) {
        seq_t cur = prev1 + ( (seq_t)(k + m - 1) * prev2 );
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

seq_t a(idx_t n)
{
    idx_t r = find_row(n);
    unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)tri;
    return compute_T(r, k);
}
