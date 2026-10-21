/*
 * A216191  Triangle T read by rows: T(n,k) equals the Manhattan distance
 *          between n and k when integers are arranged in a spiral.
 * https://oeis.org/A216191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The triangle T is read row‑by‑row.  
   T(n,k) = Manhattan distance between the positions of the integers n
   and k in a square spiral that starts at 0 in the centre and steps
   right, up, left, left, down, down, right, …  The global index n
   is interpreted as the n‑th element of that triangle:
   find r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and set k = n - r*(r+1)/2.
   The function returns |x_r - x_k| + |y_r - y_k|. */

#include <stdint.h>

/* compute (x,y) coordinates of integer m in the spiral */
static inline void coords(idx_t m, int64_t *x, int64_t *y)
{
    if (m == 0) {
        *x = 0;
        *y = 0;
        return;
    }

    /* ring index r: smallest r with 4*r*(r+1) ≥ m */
    idx_t r = 0;
    while ((__uint128_t)4 * r * (r + 1) < m) ++r;

    /* first value in ring r */
    idx_t start_val = (idx_t)(((__uint128_t)4 * (r - 1) * r) + 1);
    idx_t d = m - start_val;           /* offset inside the ring   */

    int64_t side = 2 * (int64_t)r;
    int64_t d64  = (int64_t)d;

    if (d64 < side) {                     /* upper side */
        *x = (int64_t)r;
        *y = -((int64_t)r) + 1 + d64;
    } else if (d64 < 2 * side) {          /* left side */
        *x = (int64_t)r - 1 - (d64 - side);
        *y = (int64_t)r;
    } else if (d64 < 3 * side) {          /* down side */
        *x = -((int64_t)r);
        *y = (int64_t)r - 1 - (d64 - 2 * side);
    } else {                              /* right side */
        *x = -((int64_t)r) + 1 + (d64 - 3 * side);
        *y = -((int64_t)r);
    }
}

seq_t a(idx_t n)
{
    /* locate the row r and offset k within that row */
    idx_t r = 0;
    while ((__uint128_t)(r + 1) * (r + 2) <= (__uint128_t)2 * n) ++r;

    idx_t start = (idx_t)(((__uint128_t)r * (r + 1)) / 2);
    idx_t k = n - start;

    /* coordinates of the two numbers */
    int64_t xr, yr, xk, yk;
    coords(r, &xr, &yr);
    coords(k, &xk, &yk);

    int64_t dx = (xr > xk) ? (xr - xk) : (xk - xr);
    int64_t dy = (yr > yk) ? (yr - yk) : (yk - yr);
    return (seq_t)(dx + dy);
}
