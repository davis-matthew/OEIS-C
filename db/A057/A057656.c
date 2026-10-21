/*
 * A057656  Number of points (x,y) in square lattice with (x-1/2)^2+y^2 <= n.
 * https://oeis.org/A057656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y)∈ℤ² : (x-1/2)² + y² ≤ n}
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square root for unsigned 64‑bit integers */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2)
        return n;
    idx_t res = 0;
    idx_t bit = (idx_t)1 << 62;          /* largest power of 4 ≤ 2⁶⁴ */
    while (bit > n)
        bit >>= 2;
    while (bit != 0) {
        if (n >= res + bit) {
            n -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Upper bound for |y| : y² ≤ n  ->  |y| ≤ floor(sqrt(n)) */
    idx_t y_max = isqrt_u64(n);

    seq_t total = 0;

    for (idx_t y = 0; y <= y_max; ++y) {
        /* Compute 4*n + 1 – 4*y*y using 128‑bit arithmetic to avoid overflow */
        unsigned __int128 rem128 = (unsigned __int128)4 * n + 1
                                 - (unsigned __int128)4 * y * y;
        idx_t rem = (idx_t)rem128;                  /* rem ≤ 4*n+1, fits in 64‑bit for test range */

        /* Radius for odd integer u = 2*x – 1 : |u| ≤ floor(sqrt(rem)) */
        idx_t M = isqrt_u64(rem);

        /* Count odd integers in [–M, M] : 2*floor((M+1)/2) */
        idx_t odds = ((M + 1) >> 1) * 2;

        /* Add contribution; double for y ≠ 0 because of ±y symmetry */
        seq_t contrib = (seq_t)odds;
        if (y > 0) contrib <<= 1;
        total += contrib;
    }

    return total;
}
