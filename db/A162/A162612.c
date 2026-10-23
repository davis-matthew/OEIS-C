/*
 * A162612  Triangle read by rows in which row n lists n terms, starting with
 *          n^2+n-1, with gaps = n^2-1 between successive terms.
 * https://oeis.org/A162612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = n² + n - 1 + (n - row_start - 1)*(row_start² - 1)
 * Range:     u8 n <= 26, u16 n <= 858, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where row_start is the smallest r with r(r+1)/2 ≥ n. */

#include <math.h>

seq_t a(idx_t n)
{
    /* The sequence is 1‑indexed; n >= 1.  For safety return 0 for n==0. */
    if (n == 0) return 0;

    /* Find the row r that contains the n‑th element.
       Solve r(r+1)/2 ≥ n  ->  r ≈ (√(8n+1) – 1)/2.   */
    long double discd  = 8.0L * (long double)n + 1.0L;
    long double sqrt_disc = sqrtl(discd);
    idx_t r = (idx_t)((sqrt_disc - 1.0L) / 2.0L);

    /* Adjust r in case the floating‑point estimate was low or high. */
    while (((__int128)r * (r + 1) / 2) < n) r++;
    while (r > 1 && (((__int128)(r - 1) * r) / 2) >= n) r--;

    /* Previous row total (number of terms before current row) */
    idx_t prev_total = (r - 1) * r / 2;
    /* Position within the row (0‑based) */
    idx_t pos = n - prev_total - 1;

    /* Compute values using 128‑bit arithmetic to avoid overflow,
       then cast to seq_t (which may be narrow or wide). */
    __int128 r_sq128 = (__int128)r * r;
    seq_t first  = (seq_t)(r_sq128 + r - 1);       /* first term of row r */
    seq_t gap    = (seq_t)(r_sq128 - 1);           /* gap between terms in row r */
    __int128 term_plus = (__int128)gap * (idx_t)pos;
    seq_t result = first + (seq_t)term_plus;

    return result;
}
