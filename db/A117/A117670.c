/*
 * A117670  Triangle read by rows: partial sums of the Pascal triangle minus
 *          1.
 * https://oeis.org/A117670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Σ_{m=1}^{k} C(r, m) \ where r = minimal integer with
 *            r(r+1)/2 ≥ n, \ and k = n – r(r‑1)/2 (the column index in row r)
 * Range:     u8 n <= 40, u16 n <= 144, u32 n <= 543, u64 n <= 1830,
 *            u128 n <= 1830, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n. */
    idx_t r = 1;
    unsigned __int128 sum = 1;          /* total elements up to current row */
    while (sum < (unsigned __int128)n) {
        ++r;
        sum += r;
    }

    /* Elements before this row: r(r-1)/2 = sum - r. */
    idx_t prev = (idx_t)(sum - r);
    idx_t k = n - prev;                 /* 1‑based column index */

    /* Compute the partial binomial sum Σ_{m=1}^{k} C(r, m). */
    seq_t total = 0;
    seq_t bin    = 1;                    /* C(r, 0) */
    for (idx_t m = 1; m <= k; ++m) {
        bin = bin * ((seq_t)r - (seq_t)m + 1) / (seq_t)m; /* C(r, m) */
        total += bin;
    }
    return total;
}
