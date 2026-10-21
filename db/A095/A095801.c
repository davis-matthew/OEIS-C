/*
 * A095801  Square of Narayana triangle A001263: View A001263 as a lower
 *          triangular matrix. Then the square of that matrix is also lower
 *          triangular. Sequence gives this lower triangle, read by rows.
 * https://oeis.org/A095801
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, tabl
 * Formula:   a(n) is the entry (row r, column c) of the square of the
 *            Narayana triangle A001263. The value is a(n) = Σ_{i=c}^{r}
 *            Narayana(r,i) * Narayana(i,c) where Narayana(n,k) =
 *            binomial(n,k) * binomial(n,k-1) / n. The index n is mapped to
 *            (r,c) by enumerating the lower triangular matrix row‑by‑row
 *            (triang...
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

static seq_t narayana(idx_t n, idx_t k)
{
    /* Compute Narayana(n,k) = binomial(n,k) * binomial(n,k-1) / n.
       Division is exact but must be performed after the product. */
    seq_t a = binomial(n, k);
    seq_t b = binomial(n, k - 1);
    return (a * b) / (seq_t)n;
}

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 >= n (1‑based). */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Number of terms before this row. */
    idx_t prev_total = ((r - 1) * r) / 2;
    /* Column index within the row.  c ranges from 1 to r. */
    idx_t c = n - prev_total;

    seq_t sum = 0;
    for (idx_t i = c; i <= r; ++i) {
        seq_t term1 = narayana(r, i);
        seq_t term2 = narayana(i, c);
        sum += term1 * term2;
    }
    return sum;
}
