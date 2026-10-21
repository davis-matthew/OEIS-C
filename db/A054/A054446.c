/*
 * A054446  Triangle of partial row sums of triangle A037027(n,m), n >= m >= 0
 *          (Fibonacci convolution triangle).
 * https://oeis.org/A054446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054446: Triangle of partial row sums of Fibonacci convolution triangle.
   The lower‑triangular array T(n,m) (0‑based) is defined by
         T(0,0) = 1
         T(1,0) = 2,  T(1,1) = 1
         T(n,0) = 2*T(n-1,0) + T(n-2,0)          (n ≥ 2)
         T(n,m) = T(n-1,m-1) + T(n-1,m) + T(n-2,m)  (n ≥ 2, m > 0)
   The sequence a(n) is obtained by flattening T row by row:
         a(n) = T(r,c)  where r*(r+1)/2 + c = n,  0 ≤ c ≤ r.
*/

#include <stdbool.h>

/* Number of rows pre‑allocated for caching.
   200 rows cover n up to about 10 000 (mid‑row 141). */
#define MAX_ROWS 200

seq_t a(idx_t n)
{
    /* --- determine row r and column c for index n ------------------- */
    idx_t r = 0;
    /* locate first r s.t. r*(r+1)/2 > n, then decrement */
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    r--;                                      /* correct row index   */
    idx_t c = n - (r * (r + 1)) / 2;           /* column within row  */

    /* --- static cache for the triangle -------------------------------- */
    static bool initialized = false;
    static seq_t triangle[MAX_ROWS][MAX_ROWS];
    static idx_t last_row = 0;                /* highest computed row */

    if (!initialized) {
        /* initialise the first two rows explicitly */
        triangle[0][0] = 1;                   /* T(0,0) */
        triangle[1][0] = 2;                   /* T(1,0) */
        triangle[1][1] = 1;                   /* T(1,1) */
        last_row = 1;
        initialized = true;
    }

    /* If the requested row is outside the pre‑allocated range,
       the function returns 0 (this situation should not occur for
       the allowed maximum n).                               */
    if (r >= MAX_ROWS)
        return 0;

    /* --- compute missing rows up to r -------------------------------- */
    for (idx_t i = last_row + 1; i <= r; ++i) {
        /* first element of row i */
        triangle[i][0] = 2 * triangle[i-1][0] + triangle[i-2][0];

        /* remaining elements */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t sum = triangle[i-1][k-1];
            if (k <= i-1)          /* T(i-1,k) exists */
                sum += triangle[i-1][k];
            if (k <= i-2)          /* T(i-2,k) exists */
                sum += triangle[i-2][k];
            triangle[i][k] = sum;
        }
    }
    last_row = r > last_row ? r : last_row;

    return triangle[r][c];
}
