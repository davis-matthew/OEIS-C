/*
 * A212207  Triangle read by rows: coefficients of polynomials p_{n,n-1}(x)
 *          arising in enumeration of two-line arrays.
 * https://oeis.org/A212207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient T(r,m) of the triangle, where r =
 *            floor((sqrt(8*n+1)-1)/2) (row index) m = n - r*(r+1)/2 (column
 *            in the row) T(r,m) = Σ_{k=0..m, k even} (k+1)/(r+1) * C(r+1,
 *            m+1) * C(r+1, m-k)
 * Range:     u8 n <= 16, u16 n <= 48, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* integer square root of a small value (8*n+1) */
    idx_t v = 8*n + 1;
    idx_t sroot = 0;
    for (idx_t i = 0; i * i <= v; ++i)
        sroot = i;

    idx_t r = (sroot - 1) / 2;              /* row index */
    idx_t tri = r * (r + 1) / 2;            /* terms before this row */
    idx_t m = n - tri;                      /* column in the row (0‑based) */

    idx_t s = r + 1;                        /* used in binomials & denominator */

    seq_t binom = 1;   /* C(s,0) */
    seq_t sum   = 0;

    for (idx_t j = 0; j <= m; ++j) {
        if ((j & 1) == (m & 1)) {           /* k = m - j even */
            seq_t factor = (seq_t)(m - j + 1);   /* (m-j+1) */
            sum += factor * binom;
        }
        if (j < m + 1) {                    /* compute C(s, j+1) for next loop */
            binom = binom * (seq_t)(s - j) / (seq_t)(j + 1);
        }
    }

    seq_t pref    = binom;                  /* C(s, m+1) */
    seq_t result  = sum * pref;
    result /= (seq_t)s;                     /* divide by (r+1) */
    return result;
}
