/*
 * A130684  Triangle read by rows: T(n,k) = number of squares (not necessarily
 *          orthogonal) all of whose vertices lie in an (n + 1) X (k + 1)
 *          square lattice.
 * https://oeis.org/A130684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row, k the column (1‑based), and
 *            T(n,k) = k*(k+1)*(k+2)*(2*n - k + 1)/12 (for k ≤ n)
 * Range:     u8 n <= 27, u16 n <= 210, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is generated row by row:
   1;
   2, 6;
   3,10,20;
   4,14,30,50; … */

seq_t a(idx_t n)
{
    if (n == 0)   /* not used, offset 1, but safe for completeness */
        return 0;

    idx_t pos = 0;              /* current position in the sequence */
    for (idx_t row = 1; ; ++row) {   /* iterate rows */
        for (idx_t k = 1; k <= row; ++k) {   /* iterate columns in the row */
            ++pos;
            if (pos == n) {
                /* compute T(row,k) = k*(k+1)*(k+2)*(2*row - k + 1)/12   */
                unsigned __int128 p = 0;
                p = (unsigned __int128)k;
                p = p * (k + 1);
                p = p * (k + 2);
                p = p * ((unsigned __int128)2 * row - k + 1);
                p = p / 12;  /* division exact because the formula is integer */
                return (seq_t)p;
            }
        }
    }
}
