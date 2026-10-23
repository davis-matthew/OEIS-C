/*
 * A189812  Number of right triangles on a (n+1)X8 grid.
 * https://oeis.org/A189812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 56*n^2 + 1480*n - 12216 for n > 48 (empirical)
 * Range:     u8 n <= 1, u16 n <= 26, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n > 48)
    {
        seq_t nn = (seq_t)n;
        seq_t res = (seq_t)56 * nn * nn + (seq_t)1480 * nn - 12216;
        return res;
    }

    size_t width  = (size_t)n + 1;   /* number of columns (x‑coordinates) */
    size_t height = 8;               /* number of rows   (y‑coordinates) */
    size_t total  = width * height;  /* total number of grid points */

    /* store the coordinates of all points */
    int xs[400], ys[400];
    for (size_t i = 0; i < total; ++i)
    {
        xs[i] = (int)(i % width);
        ys[i] = (int)(i / width);
    }

    seq_t count = (seq_t)0;

    /* brute‑force all triples of distinct points */
    for (size_t i = 0; i + 2 < total; ++i)
    {
        for (size_t j = i + 1; j + 1 < total; ++j)
        {
            int dxij = xs[i] - xs[j];
            int dyij = ys[i] - ys[j];
            int dij  = dxij * dxij + dyij * dyij;

            for (size_t k = j + 1; k < total; ++k)
            {
                int dxik = xs[i] - xs[k];
                int dyik = ys[i] - ys[k];
                int dik  = dxik * dxik + dyik * dyik;

                int dxjk = xs[j] - xs[k];
                int dyjk = ys[j] - ys[k];
                int djk  = dxjk * dxjk + dyjk * dyjk;

                /* check for a right angle: Pythagoras in any order */
                if (dij + dik == djk ||
                    dij + djk == dik ||
                    dik + djk == dij)
                {
                    ++count;
                }
            }
        }
    }

    return count;
}
