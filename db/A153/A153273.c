/*
 * A153273  Triangle read by rows: T(n,k) = Product_{i=0..k-2} (i*n + n - 1).
 * https://oeis.org/A153273
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read from triangle T(r,k) = ∏_{i=0}^{k-2} (i*r + r - 1),
 *            where r is the minimal integer ≥2 such that (r-1)·r/2 ≥ n-1, and
 *            k = (n - 1 - r(r-2)/2) + 2
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 66, u128 n <= 171,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(2) is the first term (row 2, column 1). */
    if (n == 2)
        return (seq_t)1;

    /* Find the row r (r ≥ 3) that contains term a(n). */
    idx_t target = n - 1;          /* we need (r-1)*r/2 ≥ target */
    idx_t low = 3, high = 3;
    while ((unsigned __int128)high * (high - 1) / 2 < target) {
        high *= 2;
    }
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid - 1) / 2 >= target)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                 /* the row number */

    /* Total terms before the start of row r */
    idx_t prev_total = 1 + (r - 1) * (r - 2) / 2;

    /* Position within the row (0‑based) */
    idx_t pos = n - prev_total - 1;   /* pos ∈ [0, r-2] */

    /* Compute the product for columns k = 2 .. pos+2 */
    seq_t prod = (seq_t)1;
    for (idx_t j = 0; j <= pos; ++j) {
        seq_t term = (seq_t)j * (seq_t)r + ((seq_t)r - (seq_t)1);
        prod *= term;
    }

    return prod;
}
