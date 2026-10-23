/*
 * A104583  Triangle read by rows: T(i,j) is the (i,j)-entry (1 <= j <= i) of
 *          the product A*B of the matrices A = [1; 3,1; 5,3,1; 7,5,3,1; ...];
 *          B = [1; 1,2; 1,2,1; 1,2,1,2; ...] (both infinite lower triangular
 *          matrices).
 * https://oeis.org/A104583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (j odd ? k*k : 2*k*k), where * k = r - j + 1, * r =
 *            floor((sqrt(8*n + 1) - 1) / 2) + 1, * j = n - (r-1)*r/2 + 1
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute row number r (1‑based). */
    double temp = sqrt((double)8 * (double)n + 1.0);
    idx_t r = (idx_t)((temp - 1.0) / 2.0);
    r++;   /* adjust to 1‑based row */

    /* Count of elements before row r. */
    unsigned __int128 prev128 = (unsigned __int128)(r - 1) * r / 2;
    idx_t prev = (idx_t)prev128;

    /* Column index j (1‑based) inside the row. */
    idx_t j = n - prev + 1;

    /* Distance from the diagonal. */
    idx_t k = r - j + 1;

    /* Base value k^2. */
    seq_t val = (seq_t)k * (seq_t)k;

    /* Double if column j is even. */
    if ((j & 1ULL) == 0)
        val = val << 1;

    return val;
}
