/*
 * A138530  Triangle read by rows: T(n,k) = sum of digits of n in base k
 *          representation, 1<=k<=n.
 * https://oeis.org/A138530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) = T(r,k) where r = smallest integer with r(r+1)/2 ≥ n, k =
 *            n - r(r-1)/2. T(n,k) = sum of the base‑k digits of n
 * Range:     u8 n <= 9222, u16 n <= 9222, u32 n <= 9222, u64 n <= 9222,
 *            u128 n <= 9222, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n‑th element of the flattened triangle
   “Triangle read by rows: T(n,k) = sum of digits of n in base k,
   1 ≤ k ≤ n”. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t low = 1, high = n, r;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    r = low;                                   /* row number */

    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;                /* number of entries before row r */
    idx_t k = n - prev;                         /* 1 ≤ k ≤ r */

    if (k == 1)                                 /* base‑1 case: T(n,1)=n */
        return (seq_t)r;

    seq_t sum = 0;
    idx_t tmp = r;
    while (tmp) {
        sum += (seq_t)(tmp % k);
        tmp /= k;
    }
    return sum;
}
