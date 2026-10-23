/*
 * A117446  Triangle T(n, k) = binomial(L(k/3), n-k) where L(j/p) is the
 *          Legendre symbol of j and p, read by rows.
 * https://oeis.org/A117446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = T(r,k) where T(r,k) = binomial(L(k/3), r-k), with L(k/3)
 *            the Legendre symbol modulo 3. r is the row index such that
 *            offset_row(r) <= n < offset_row(r+1), offset_row(r) = r*(r+1)/2,
 *            and k = n - offset_row(r)
 * Range:     i8 n <= 1325, i16 n <= 1325, i32 n <= 1325, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function is O(sqrt(n)) and does not use any external tables. */

seq_t a(idx_t n)
{
    /* Find the row r containing the linear index n.
       offset_row(r) = r*(r+1)/2.  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        r++;

    /* Starting index of this row */
    idx_t offset = r * (r + 1) / 2;
    idx_t k = n - offset;          /* column index within the row   */
    idx_t diff = r - k;            /* r - k  (non‑negative because k <= r) */
    idx_t mod3 = k % 3;

    switch (mod3)
    {
        case 0:   /* L = 0: binomial(0, diff) = 1 if diff==0 else 0 */
            return (diff == 0) ? (seq_t)1 : (seq_t)0;

        case 1:   /* L = 1: binomial(1, diff) = 1 if diff==0 or diff==1 */
            return (diff == 0 || diff == 1) ? (seq_t)1 : (seq_t)0;

        case 2:   /* L = -1: binomial(-1, diff) = (-1)^diff */
            return (diff & 1) ? (seq_t)(-1) : (seq_t)1;

        default:
            return (seq_t)0;   /* unreachable */
    }
}
