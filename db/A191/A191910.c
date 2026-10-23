/*
 * A191910  Triangle read by rows: T(n,n)=n; T(n,k) = k-1 if k divides n and k
 *          < n, otherwise -1.
 * https://oeis.org/A191910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   a(n) = entry at (r,k) where * r = smallest integer with
 *            r*(r+1)/2 >= n; * k = n - r*(r-1)/2; * if k==r return r; * else
 *            if r % k == 0 return k-1; * else return -1
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the triangle T(n,k) with T(n,n)=n, T(n,k)=k-1 if k|n, else -1,
 * read row‑wise to form the sequence. */

#include <stdint.h>

/* Compute r such that r*(r+1)/2 >= n, using binary search and 128‑bit arithmetic. */
static idx_t row_of(idx_t n) {
    idx_t lo = 1, hi = n;
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __uint128_t t = (__uint128_t)mid * (mid + 1) / 2;
        if (t >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

seq_t a(idx_t n) {
    idx_t r = row_of(n);
    __uint128_t prev_tri = (__uint128_t)(r - 1) * r / 2; /* sum of rows before r */
    idx_t k = (idx_t)(n - prev_tri - 1) + 1; /* column index, 1‑based */

    if (k == r) {
        return (seq_t)r;
    } else if (r % k == 0) {
        return (seq_t)(k - 1);
    } else {
        return (seq_t)-1;
    }
}
