/*
 * A236540  Triangle read by rows: T(n,k), n>=1, k>=1, in which column k lists
 *          k copies of the positive squares in nondecreasing order, except
 *          the first column which lists the triangular numbers, and the first
 *          element of column k is in row k(k+1)/2.
 * https://oeis.org/A236540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A236540: triangle read by rows
 *  T(n,k) row n (n≥1), column k (k≥1)
 *  • Column 1 contains the triangular numbers T_{n-1} = (n-1)n/2
 *  • For k≥2, column k lists each square number m² exactly k times, in order;
 *    the first row where column k appears is n = k(k+1)/2.
 *  The sequence a(n) is the n‑th term when the triangle is read row by row.
 *
 *  The function below computes a(n) for any n≥1 using only integer
 *  arithmetic; it scans rows until the cumulative length exceeds n,
 *  then finds the column k and returns the appropriate value.
 */

#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned value.
   Uses binary search and unsigned __int128 to avoid overflow. */
static inline idx_t isqrt_uint64(idx_t x)
{
    idx_t lo = 0;
    idx_t hi = (idx_t)1ULL << 32;   /* ≥ √(max idx_t) */
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == (unsigned __int128)x)
            return mid;
        if (sq < (unsigned __int128)x) {
            lo = mid + 1;
        } else {
            if (mid == 0) break;
            hi = mid - 1;
        }
    }
    return hi;   /* floor(sqrt(x)) */
}

seq_t a(idx_t n)
{
    if (n == 0)   /* safety; sequence is defined for n≥1 */
        return 0;

    idx_t acc = 0;   /* cumulative length of rows processed so far */
    idx_t r   = 1;   /* current row number */

    while (1) {
        /* L(r) = floor((sqrt(8r+1)-1)/2)  – number of columns in row r */
        idx_t L = (isqrt_uint64(8*r + 1) - 1) / 2;
        if (acc + L >= n)
            break;
        acc += L;
        ++r;
    }

    idx_t k = n - acc;   /* 1‑based column index within row r */

    if (k == 1) {   /* first column: triangular numbers */
        idx_t tri = (r - 1) * r / 2;
        return (seq_t)tri;
    }

    /* columns k≥2: repeated squares */
    idx_t S_k = k * (k + 1) / 2;   /* first row where column k appears */
    idx_t d   = r - S_k;           /* zero‑based offset from that start */
    idx_t m   = d / k + 1;         /* square index (1‑based) */
    seq_t val = (seq_t)m * (seq_t)m;
    return val;
}
