/*
 * A162616  Triangle read by rows in which row n lists n terms, starting with
 *          n^3 + n - 1, such that the difference between successive terms is
 *          equal to n^3 - 1 = A068601(n).
 * https://oeis.org/A162616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = r³ + r – 1 + (k – 1)*(r³ – 1), where r = ceil((√(8n + 1)
 *            – 1) / 2) and k = n – r(r – 1)/2
 * Range:     u8 n <= 9, u16 n <= 135, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* undefined for the offset, return 0 */

    /* Find the row r such that r*(r+1)/2 ≥ n.  Binary search on r. */
    idx_t low = 1;
    idx_t high = 1;
    while (high * (high + 1) / 2 < n) high <<= 1;   /* guaranteed to overshoot */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;                     /* row number (1‑based) */

    /* Position k within this row (1‑based). */
    idx_t prev = (r - 1) * r / 2;       /* number of terms before row r */
    idx_t k = n - prev;                 /* 1‑based index inside the row */

    /* Compute the term using seq_t arithmetic only. */
    seq_t r3 = (seq_t)r;                /* r³ */
    seq_t r3_cub = r3 * r3 * r3;        /* r³ */
    seq_t diff   = r3_cub - 1;          /* common difference r³ – 1 */
    seq_t first  = r3_cub + r3 - 1;     /* first term of the row   */
    seq_t val    = first + (k - 1) * diff;

    return val;
}
