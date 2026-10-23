/*
 * A048149  Array T read by diagonals: T(i,j) = number of pairs (h,k) with
 *          h^2+k^2 <= i^2+j^2, h>=0, k >= 0.
 * https://oeis.org/A048149
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A048149.
   a(n) is obtained by reading the array T(i,j) by diagonals:
   T(i,j) = number of pairs (h,k), h>=0,k>=0, with h^2 + k^2 <= i^2 + j^2.
   For a given n, determine the diagonal k and position offset, map to
   (i,j) and count the lattice points in the first quadrant within the
   circle of radius sqrt(i^2 + j^2). */
#include <stdint.h>
#include <stddef.h>

/* integer square root of a 64‑bit unsigned value: floor(sqrt(n)) */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n <= 1)
        return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;       /* initial guess: (n+1)/2 */
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* OEIS A048149 */
seq_t a(idx_t n)
{
    /* find the diagonal r containing term n:
       r such that r(r+1)/2 <= n < (r+1)(r+2)/2   */
    idx_t d = 8 * n + 1;
    idx_t sqrt_d = isqrt_u64(d);
    idx_t r = (sqrt_d - 1) >> 1;        /* floor((sqrt(8n+1)-1)/2) */
    idx_t prev = r * (r + 1) / 2;       /* terms before diagonal r */
    /* offset within this diagonal (0..r) */
    idx_t offset = n - prev;
    idx_t i = offset;          /* horizontal coordinate */
    idx_t j = r - offset;      /* vertical coordinate */

    /* squared radius */
    idx_t R2 = i * i + j * j;

    /* count integer lattice points (h,k) >= 0 with h^2 + k^2 <= R2 */
    seq_t count = 0;
    idx_t hmax = isqrt_u64(R2);
    idx_t kmax = hmax;            /* start with kmax for h = 0 */

    for (idx_t h = 0; h <= hmax; ++h) {
        idx_t rem = R2 - h * h;
        /* adjust kmax to floor(sqrt(rem)) */
        while (kmax * kmax > rem)
            --kmax;
        count += (seq_t)(kmax + 1);
    }

    return count;
}
