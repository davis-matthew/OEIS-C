/*
 * A108285  Triangle read by rows, generated from (1, 2, 3, ...).
 * https://oeis.org/A108285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if n is of the form r*(r+1)/2 (start of a row);
 *            otherwise a(n) = Σ_{k=0}^{c} (k+1) * (r - c + 1)^{c-k} where r
 *            is the row index (0‑based) such that r*(r+1)/2 <= n <
 *            (r+1)*(r+2)/2, and c = n - r*(r+1)/2 is the column within that
 *            row
 * Range:     u8 n <= 31, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Determine the row r that contains index n */
    idx_t r = 0;
    while (1) {
        unsigned __int128 limit = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (limit > n) break;
        r++;
    }

    idx_t prev = r * (r + 1) / 2;        /* number of entries before this row */
    idx_t c = n - prev;                 /* column within the row */

    if (c == 0)                         /* first element of a row is always 1 */
        return (seq_t)1;

    idx_t m = r - c + 1;                /* value of x for the column's polynomial */
    idx_t col = c;                      /* number of terms in the sum (= c) */

    /* Compute power = m^col */
    seq_t power = 1;
    seq_t s_m = (seq_t)m;
    for (idx_t i = 0; i < col; ++i) {
        power *= s_m;
    }

    /* Sum Σ_{k=0}^{col} (k+1)*m^{col-k} */
    seq_t result = 0;
    for (idx_t k = 0; k <= col; ++k) {
        result += (seq_t)(k + 1) * power;
        power /= s_m;                 /* m divides power exactly */
    }

    return result;
}
