/*
 * A174148  Triangle read by rows: T(n,k) =
 *          binomial(n,k)*(binomial(n-1,k-1)*binomial(n+1,k+1) +
 *          binomial(n-1,k)*binomial(n+1,k)), with T(0,0) = 1.
 * https://oeis.org/A174148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 96, u64 n <= 310,
 *            u128 n <= 1100, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A174148 – Triangle read by rows
   Formula (for n>0):  T(n,k)=C(n,k)*(C(n‑1,k‑1)*C(n+1,k+1)+C(n‑1,k)*C(n+1,k)),
   row r: r*(r+1)/2 ≤ i < (r+1)*(r+2)/2,  column k=i−r*(r+1)/2,
   with T(n,0)=T(n,n)=1.
   a(n)=T(row, column).  a(0)=1. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return 1;

    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2
       using a binary search on r.  Use 128‑bit arithmetic for safety. */
    idx_t r = 0, low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }

    /* Column index within the row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;

    /* Boundary values are 1 */
    if (k == 0 || k == r)
        return 1;

    /* Compute the triangle entry T(r,k) */
    seq_t bn    = binomial(r, k);
    seq_t part1 = binomial(r - 1, k - 1) * binomial(r + 1, k + 1);
    seq_t part2 = binomial(r - 1, k)     * binomial(r + 1, k);

    return bn * (part1 + part2);
}
