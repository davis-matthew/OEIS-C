/*
 * A143808  Triangle read by rows, first n terms of the sequence (1, 2, 4, 10,
 *          ...) followed by P(n), where (2, 4, 10, 12, ...) = twice Pell
 *          numbers.
 * https://oeis.org/A143808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = { * 1 if n = r(r-1)/2 (first element of row r) *
 *            Pell(r-1) if n = r(r+1)/2-1 (last element of row r) * 2 * Pell(n
 *            - r(r-1)/2) otherwise (middle elements) * } where Pell(k) is the
 *            k-th Pell number (Pell(0)=0, Pell(1)=1,
 *            Pell(k)=2*Pell(k-1)+Pell(k-2))
 * Range:     u8 n <= 42, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pell_at(idx_t k) {
    if (k == 0) return 0;
    if (k == 1) return 1;
    seq_t p0 = 0;
    seq_t p1 = 1;
    for (idx_t i = 2; i <= k; ++i) {
        seq_t pk = (p1 << 1) + p0;  /* 2*Pell(i-1) + Pell(i-2) */
        p0 = p1;
        p1 = pk;
    }
    return p1;
}

seq_t a(idx_t n)
{
    /* find the triangle row r and the position pos within that row */
    idx_t sum = 0;              /* total number of elements before the current row   */
    idx_t r   = 1;              /* current row number (length of the row)             */
    while (r <= n - sum) {      /* while the next row would not exceed n             */
        sum += r;               /* add the current row length to the cumulative sum  */
        ++r;                    /* move to the next row                            */
    }
    idx_t pos = n - sum;        /* 0‑based index within the row r                    */

    if (pos == 0) {             /* first element of every row is always 1            */
        return 1;
    }
    if (pos == r - 1) {         /* last element of the row                           */
        return pell_at(r - 1);
    }
    return (pell_at(pos) << 1); /* middle elements are twice the Pell number        */
}
