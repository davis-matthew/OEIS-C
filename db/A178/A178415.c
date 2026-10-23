/*
 * A178415  Array T(n,k) of odd Collatz preimages read by antidiagonals.
 * https://oeis.org/A178415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   A178415 is read by antidiagonals of the array T(n,k). For a
 *            given index n (1‑based) find the pair (row, col): let m be the
 *            smallest integer with m(m+1)/2 >= n, pos = n - (m-1)m/2, row = m
 *            - pos + 1, col = pos. Then T(row,col) is computed by if row==1 :
 *            T = T(1,col) if row==2 : T = T(2,col) if row...
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 152, u64 n <= 560,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Locate the (row,col) pair for the nth term */
    idx_t m = 1;
    while (m * (m + 1) / 2 < n) ++m;                 /* small m for n<=10^4 */
    idx_t prev_tri = (m - 1) * m / 2;                 /* (m-1)m/2 */
    idx_t pos = n - prev_tri;                        /* 1..m */
    idx_t row = m - pos + 1;                         /* 1..m */
    idx_t col = pos;                                 /* 1..m */

    /* Compute 4^col, T(1,col) and T(2,col) */
    seq_t pow4k = 4;   /* 4^1 */
    seq_t t1 = 1;      /* T(1,1) */
    seq_t t2 = 3;      /* T(2,1) */
    if (col > 1) {
        for (idx_t i = 2; i <= col; ++i) {
            pow4k = pow4k * 4;          /* 4^i  (i starts from 2) */
            t1 = 4 * t1 + 1;            /* recurrence for col >= 2 */
            t2 = 4 * t2 + 1;
        }
    }

    if (row == 1) return t1;
    if (row == 2) return t2;

    if (row & 1) {   /* row odd and >=3 */
        seq_t mul = (row - 1) / 2;
        seq_t add = mul * (pow4k << 1);   /* 2 * 4^col = 2^(2col+1) */
        return t1 + add;
    } else {         /* row even and >=4 */
        seq_t mul = (row - 2) / 2;
        seq_t add = mul * pow4k;          /* 4^col            */
        return t2 + add;
    }
}
