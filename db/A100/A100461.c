/*
 * A100461  Triangle read by rows, based on array described below.
 * https://oeis.org/A100461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(k, m) where k is the column number (k*(k+1)/2 >= n), m
 *            = k - (n - (k-1)*k/2 - 1) and T is defined by T(1, k) = 2^(k-1),
 *            T(m+1, k) = (k-m) * floor((T(m, k)-1)/(k-m)) (1 ≤ m < k)
 * Range:     u8 n <= 36, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the column k such that cumulative count up to k is >= n */
    idx_t k = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++k;
        cumulative += k;
    }
    /* cumulative now equals k*(k+1)/2, cumulative_before = (k-1)*k/2 */
    idx_t cumulative_before = cumulative - k;
    idx_t offset_in_col = n - cumulative_before - 1; /* 0-based within column */
    idx_t m = k - offset_in_col;                     /* row index (1‑based) */

    /* Compute T(1, k) = 2^(k-1) via iterative doubling to avoid overflow of shift */
    seq_t t_current = 1;
    for (idx_t i = 0; i < k - 1; ++i) {
        t_current <<= 1; /* multiply by 2 */
    }

    /* If we already need the first row, return immediately */
    if (m == 1)
        return t_current;

    /* Iterate the recurrence up to row m */
    for (idx_t i = 1; i < m; ++i) {
        seq_t diff = (seq_t)(k - i);                      /* (k - m) part */
        seq_t quotient = (t_current - 1) / diff;          /* floor division */
        t_current = diff * quotient;                      /* next row value */
    }
    return t_current;
}
