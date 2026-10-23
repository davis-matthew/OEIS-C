/*
 * A189810  Number of right triangles on a (n+1)X6 grid.
 * https://oeis.org/A189810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 30*n^2 + 468*n - 2478 for n > 24 For 1 ≤ n ≤ 24 the value
 *            is computed by brute‑force enumeration of all unordered triples
 *            of grid points in an (n+1)×6 lattice
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Apply the polynomial for large n */
    if (n > 24)
    {
        seq_t sn = (seq_t)n;
        return (seq_t)30 * sn * sn + (seq_t)468 * sn - 2478;
    }

    /* Brute‑force counting for the small cases */
    const idx_t width  = n + 1;   /* columns 0 … n  */
    const idx_t height = 6;       /* rows    0 … 5  */
    const idx_t m = width * height;   /* total lattice points  */

    /* Max m for n = 24 is 150 – small array is fine */
    int xs[150], ys[150];
    idx_t idx = 0;
    for (idx_t x = 0; x < width; ++x)
        for (idx_t y = 0; y < height; ++y)
        {
            xs[idx] = (int)x;
            ys[idx] = (int)y;
            ++idx;
        }

    seq_t count = 0;
    for (idx_t i = 0; i < m; ++i)
    {
        const int xi = xs[i], yi = ys[i];
        for (idx_t j = i + 1; j < m; ++j)
        {
            const int xj = xs[j], yj = ys[j];
            const int dxij = xj - xi;
            const int dyij = yj - yi;

            for (idx_t k = j + 1; k < m; ++k)
            {
                const int xk = xs[k], yk = ys[k];
                const int dxik = xk - xi;
                const int dyik = yk - yi;
                const int dxjk = xk - xj;   /* from j to k   */
                const int dyjk = yk - yj;   /* from j to k   */

                /* Check 90° at i, j, or k */
                const int dot_i = dxij * dxik + dyij * dyik;
                const int dot_j = dxij * (-dxjk) + dyij * (-dyjk);
                const int dot_k = dxik * dxjk + dyik * dyjk;

                if (dot_i == 0 || dot_j == 0 || dot_k == 0)
                    count += 1;
            }
        }
    }

    return count;
}
