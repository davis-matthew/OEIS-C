/*
 * A059026  Table B(n,m) read by rows: B(n,m) = LCM(n,m)/n + LCM(n,m)/m - 1
 *          for all 1<=m<=n.
 * https://oeis.org/A059026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = LCM(row, col)/row + LCM(row, col)/col - 1 where (row,
 *            col) is the position of the n‑th element when the array B(row,
 *            col) is read row by row
 * Range:     u8 n <= 82, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   triangular_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/triangular_root.h"
/* Helper: Euclidean gcd for unsigned integers. */
static inline idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* The requested sequence function. */
seq_t a(idx_t n)
{
    /* Find the row (r) and column (m) for the n‑th term. */
    idx_t r = triangular_root(n);
    idx_t prev_tri = ( (__int128)(r - 1) * r ) / 2;  /* T_{r-1} */
    idx_t m = n - prev_tri;

    /* Compute LCM(r, m) = r/gcd(r,m) * m. */
    idx_t g = gcd_idx(r, m);
    seq_t lcm = ((seq_t)(r / g)) * (seq_t)m;

    /* B(r, m) = lcm/r + lcm/m - 1. */
    seq_t result = lcm / (seq_t)r + lcm / (seq_t)m - 1;
    return result;
}
