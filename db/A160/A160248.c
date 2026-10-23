/*
 * A160248  Table read by antidiagonals of "less regular" truncated
 *          tetrahedron numbers built of face-centered-cubic sphere packing.
 * https://oeis.org/A160248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   v(x,y) = (y³ + 4x³ + 6y²x + 12yx² - 3y² - 12x² - 12yx + 2y + 8x)
 *            / 6 a(n) is obtained by reading the table of v(x,y) with x≥1,
 *            y≥1 by antidiagonals: for each s ≥ 2 list points (x,y) with x =
 *            s-1,…,1 and y = 1,…,s-1 (i.e., x = s - k, y = k for k =
 *            1,…,s-1). The offset is 1, so a(1) = v(1,1). This...
 * Range:     u8 n <= 6, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1, but guard anyway */

    /* Find s such that s(s-1)/2 >= n  (minimal such s) */
    idx_t low = 1;
    idx_t high = n + 1;        /* s never exceeds n+1 for n >= 1 */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t cum = ((seq_t)mid * (mid - 1)) / (seq_t)2;
        if (cum >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    /* Previous antidiagonal gives the count of terms before this one */
    seq_t prev_cnt = ((seq_t)(s - 1) * (s - 2)) / (seq_t)2;
    idx_t prev = (idx_t)prev_cnt;

    idx_t k = n - prev;        /* position within the current antidiagonal (1-based) */
    idx_t x = s - k;
    idx_t y = k;

    /* Compute v(x,y) using the formula */
    seq_t xx = (seq_t)x;
    seq_t yy = (seq_t)y;

    seq_t term = (yy * yy * yy)                       /* y³ */
               + ((seq_t)4) * xx * xx * xx             /* 4x³ */
               + ((seq_t)6) * yy * yy * xx            /* 6y²x */
               + ((seq_t)12) * yy * xx * xx           /* 12yx² */
               - ((seq_t)3) * yy * yy                 /* -3y² */
               - ((seq_t)12) * xx * xx                /* -12x² */
               - ((seq_t)12) * yy * xx                /* -12yx */
               + ((seq_t)2) * yy                      /* +2y */
               + ((seq_t)8) * xx;                    /* +8x */

    return term / ((seq_t)6);                        /* division is exact */
}
