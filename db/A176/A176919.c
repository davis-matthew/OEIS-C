/*
 * A176919  Triangle by columns: (1, 2, 3, ...) in each column interleaved
 *          with (0, 1, 2, ...) zeros. Columns > 1 shifted down twice.
 * https://oeis.org/A176919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = * let (r, c) be the row and column of the triangle that
 *            contains a(n), * where r satisfies T(r‑1) < n ≤ T(r) with
 *            T(k)=k(k+1)/2 and * c = n – T(r‑1) (1‑based). Then * if r < 2c‑1
 *            then a(n) = 0 * else if (r-(2c‑1)) mod c == 0 then a(n) =
 *            (r-(2c‑1))/c + 1 * else a(n) = 0
 * Range:     u8 n <= 44, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Binary search to find the row r such that T(r‑1) < n ≤ T(r) */
    idx_t low = 1, high = n;               /* high = n is a safe upper bound */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        seq_t tmid = (seq_t)mid * (seq_t)(mid + 1) / 2;
        if (tmid >= (seq_t)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;  /* row number, 1‑based */

    /* Compute previous triangular number T(r‑1) */
    seq_t tprev_big = (seq_t)(r - 1) * (seq_t)r / 2;
    idx_t tprev = (idx_t)tprev_big;          /* fits in idx_t because tprev_big ≤ n */

    /* Column index c (1‑based) */
    idx_t c = n - tprev;                     /* 1 ≤ c ≤ r */

    /* Triangle entry based on the definition */
    idx_t k = c;
    idx_t start = 2 * k - 1;
    if (r < start)
        return 0;

    idx_t d = r - start;
    if (d % k == 0)
        return (seq_t)(d / k + 1);

    return 0;
}
