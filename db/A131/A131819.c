/*
 * A131819  A131818 * A000012 as infinite lower triangular matrices. Triangle
 *          read by rows, partial sums starting from the right of A131818.
 * https://oeis.org/A131819
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Let r be the smallest integer with r(r+1)/2 ≥ n. Let k = n –
 *            (r(r–1)/2), 1 ≤ k ≤ r (position inside the r‑th row, 1‑based).
 *            The first element of row r is f = (r^2 + 3r – 2)/2 . Then a(n) =
 *            f if k==1 a(n) = f – r – ((k–1)k)/2 + 1 otherwise
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Find r such that r*(r+1)/2 >= n  (minimal r) */
    idx_t r = (idx_t)sqrtl(2.0L * (long double)n);
    /* Adjust by checking with 128‑bit arithmetic to avoid overflow */
    while (( (__uint128_t)r * (r + 1) ) / 2 < n) r++;
    while (r > 0 && ( (__uint128_t)(r - 1) * r ) / 2 >= n) r--;

    /* First element of row r */
    seq_t f = ((seq_t)r * (seq_t)r + (seq_t)3 * r - (seq_t)2) / (seq_t)2;

    /* Position inside the row */
    idx_t start_idx = r * (r - 1) / 2 + 1;
    idx_t pos = n - start_idx;           /* 0‑based */
    idx_t k = pos + 1;                   /* 1‑based */

    if (k == 1) {
        return f;
    }

    /* a(n) = f - r - ((k-1)*k)/2 + 1 */
    seq_t part = ((seq_t)(k - 1) * (seq_t)k) / (seq_t)2;
    seq_t result = f - (seq_t)r - part + (seq_t)1;
    return result;
}
