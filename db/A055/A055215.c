/*
 * A055215  A path-counting array, read by rows: T(i,j)=number of paths from
 *          (0,0) to (i-j,j) using steps (1 unit right and 1 unit up) or (1
 *          unit right and 2 units up).
 * https://oeis.org/A055215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, walk
 * Formula:   T(0,0)=1; T(i,0)=T(i,i)=1; T(i,1)=1 for i≥1. For i≥3 and
 *            2≤j≤i−1: T(i,j)=T(i−2,j−1)+T(i−3,j−2). a(n) is the n‑th element
 *            of the flattened triangle T read by rows, with a(1)=T(0,0)
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Maximum number of triangle cells needed for n up to ~10000. */
#define MAX_TRIAN_SIZE 11000

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find row i (0‑based) such that (i+1)(i+2)/2 ≥ n. */
    idx_t i = 0;
    while (((i + 1) * (i + 2)) / 2 < n) ++i;

    idx_t prev_rows = i * (i + 1) / 2;      /* total cells before row i */
    idx_t j = n - prev_rows - 1;           /* column index within row i */

    /* Flat buffer to hold the triangle up to row i. */
    static seq_t tri[MAX_TRIAN_SIZE];

    /* Compute rows 0 … i. */
    for (idx_t r = 0; r <= i; ++r) {
        idx_t base = r * (r + 1) / 2;
        for (idx_t c = 0; c <= r; ++c) {
            seq_t val;
            if (c == 0 || c == r || (r >= 1 && c == 1))
                val = 1;
            else if (r >= 3 && c >= 2 && c <= r - 1) {
                idx_t idx1 = (r - 2) * (r - 1) / 2 + (c - 1);
                idx_t idx2 = (r - 3) * (r - 2) / 2 + (c - 2);
                val = tri[idx1] + tri[idx2];
            } else
                val = 0;
            tri[base + c] = val;
        }
    }

    return tri[prev_rows + j];
}
