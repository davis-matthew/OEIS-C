/*
 * A120889  Triangle read by rows: T(n,k) = gcd(k,ceiling(n/k)) (1 <= k <= n).
 * https://oeis.org/A120889
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9842, u16 n <= 9842, u32 n <= 9842, u64 n <= 9842,
 *            u128 n <= 9842, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence: A120889 – Triangle read by rows.
   For a given index n ≥ 1 we first determine the row r and column k
   such that the n‑th term lies in the triangle entry
       T(r,k) = gcd(k, ceil(r/k)).
   The row indices satisfy r(r+1)/2 ≥ n > (r−1)r/2.
   The column index is k = n − (r−1)r/2 (1 ≤ k ≤ r).
*/
#include "../utils/gcd.h"

static idx_t triangular_root(idx_t n) {
    /* smallest r with r(r+1)/2 >= n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = (low + high) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n) {
    idx_t r = triangular_root(n);
    /* cumulative terms before row r */
    __uint128_t prev128 = (__uint128_t)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;          /* prev ≤ n by construction */
    idx_t k = n - prev;                   /* 1 ≤ k ≤ r */

    seq_t kd = (seq_t)k;
    /* ceil(r/k) = (r + k – 1) / k */
    seq_t ceil_val = ((seq_t)r + kd - 1) / kd;

    return gcd(kd, ceil_val);
}
