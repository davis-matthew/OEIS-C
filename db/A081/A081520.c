/*
 * A081520  Triangle read by rows in which row n consists of 1 and the n-1
 *          smallest positive numbers having common factors with n.
 * https://oeis.org/A081520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes A081520: Triangle read by rows where row n contains 
 * 1 and the n−1 smallest positive integers having a common factor with n. 
 * The function returns the n‑th term of the triangle read row‑wise. 
 * It uses a simple scan for the required number, which is efficient for the 
 * typical problem limits (n up to about 10 000). 
 */
#include <stdint.h>
#include <assert.h>

/* Find the greatest common divisor of a and b (both unsigned). */
static inline idx_t gcd_uint64(idx_t a, idx_t b)
{
    while (b) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Floor square root of a 64‑bit unsigned integer, using binary search. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = n;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if ((__int128)mid * mid <= n)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Find the row r such that T_{r-1} < n <= T_r, where T_r = r(r+1)/2. */
static inline idx_t row_for_index(idx_t n)
{
    /* binary search on r */
    idx_t lo = 1, hi = 1;
    /* exponential search to bound hi */
    while (( (__int128)hi * (hi + 1) ) / 2 < n)
        hi <<= 1;          /* double hi */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (( (__int128)mid * (mid + 1) ) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Main function to compute the term a(n) of A081520. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* Undefined for n=0, return 0 as a safe fallback */

    idx_t row = row_for_index(n);
    idx_t prev = ( (__int128)(row - 1) * row ) / 2;  /* terms before this row */
    idx_t col = n - prev;                            /* 1‑based position in the row */

    if (col == 1)
        return (seq_t)1; /* first element of every row */

    /* Need the (col‑1)‑th positive integer m≥2 with gcd(row, m) > 1. */
    idx_t target = col - 1;
    idx_t count = 0;
    idx_t m = 2;

    while (1) {
        if (gcd_uint64(row, m) > 1) {
            count++;
            if (count == target)
                return (seq_t)m;
        }
        ++m;
    }
}
