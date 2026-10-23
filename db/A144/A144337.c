/*
 * A144337  Triangle read by rows, A000012 * (2*A144328 - A000012).
 * https://oeis.org/A144337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = if c == 1 then r else (2*c - 3) * (r - c + 1) where r =
 *            ceil((sqrt(8*n-7)-1)/2) is the row index (solved by binary
 *            search) and c = n - (r-1)*r/2 is the column index in that row
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r such that T_{r-1} < n <= T_r  (T_k = k*(k+1)/2). */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t   = (idx_t)mid * (mid + 1) / 2;
        if (t < n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low;                       /* row index */
    idx_t prev = (idx_t)(r - 1) * r / 2; /* T_{r-1} */
    idx_t c = (idx_t)(n - prev);         /* column index */

    if (c == 1)
        return r;                        /* first column */
    seq_t coeff = (seq_t)(2 * c - 3);    /* (2*c - 3) */
    seq_t mult  = (seq_t)(r - c + 1);    /* (r - c + 1) */
    return coeff * mult;
}
