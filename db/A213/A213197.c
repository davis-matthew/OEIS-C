/*
 * A213197  T(n,k) = (2*(n+k)^2 - 2*(n+k) - 4*k + 6 + (2*k-2)*(-1)^n +
 *          (2*k-1)*(-1)^k + (-2*n+1)*(-1)^(n+k))/4; n, k > 0, read by
 *          antidiagonals.
 * https://oeis.org/A213197
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(i,j) where (i,j) is obtained by reading the table along
 *            antidiagonals. T(i,j) = (2*(i+j)^2 - 2*(i+j) - 4*j + 6 +
 *            (2*j-2)*(-1)^i + (2*j-1)*(-1)^j + (-2*i+1)*(-1)^(i+j))/4 ; The
 *            function below computes a(n) for n >= 1
 * Range:     u8 n <= 56, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find the diagonal sum s = i + j by accumulating the lengths
       of the antidiagonals.  The diagonal with sum s has length s-1. */
    idx_t s = 2;
    idx_t cum = 0;                /* number of terms before or on this diagonal */
    for (;;) {
        cum += s-1;                /* add length of current diagonal */
        if (cum >= n) break;       /* the required term lies on this diagonal */
        ++s;
    }

    /* Position of the term within this diagonal (1‑based) */
    idx_t prev = cum - (s-1);        /* total terms before this diagonal */
    idx_t i = n - prev;              /* row index */
    idx_t j = s - i;                 /* column index */

    /* Convert to seq_t for arithmetic. */
    seq_t tn = (seq_t)i;
    seq_t tj = (seq_t)j;
    seq_t ss = tn + tj;                /* s = i + j */

    /* Compute numerator components using non‑negative arithmetic. */
    seq_t pos = 2*ss*ss;               /* 2*(i+j)^2      */
    pos -= 2*ss;                       /* -2*(i+j)       */
    pos -= 4*tj;                       /* -4*j           */
    pos += 6;                          /* +6             */

    seq_t neg = 0;

    /* (2*j-2)*(-1)^i  */
    seq_t a_val = 2*tj - 2;            /* 2*j-2  >= 0    */
    if (i & 1) neg += a_val;          /* odd i  -> subtract  */
    else       pos += a_val;           /* even i -> add      */

    /* (2*j-1)*(-1)^j  */
    seq_t b_val = 2*tj - 1;            /* 2*j-1  >= 1    */
    if (j & 1) neg += b_val;          /* odd j  -> subtract  */
    else       pos += b_val;           /* even j -> add      */

    /* (-2*i+1)*(-1)^(i+j)  ->  -(1-2*i)*(-1)^(i+j)
       we use c_pos = 2*i-1 (positive) and add or subtract accordingly. */
    seq_t c_pos = 2*tn - 1;            /* positive value 2*i-1 */
    if (ss & 1) pos += c_pos;         /* odd (i+j) -> add  (subtract negative) */
    else        neg += c_pos;          /* even (i+j) -> subtract (add negative)   */

    /* Result is (pos - neg) / 4 */
    return (pos - neg) / 4;
}
