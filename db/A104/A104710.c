/*
 * A104710  Triangle read by rows: reversed partial sums of Narayana triangle
 *          rows.
 * https://oeis.org/A104710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   reversed partial sums of the Narayana triangle rows a(n) =
 *            sum_{i=k}^{r} N(r,i) where r(r+1)/2 >= n and k = n - r(r-1)/2,
 *            with N(r,i) = binom(r,i)*binom(r,i-1)/r The function implements
 *            this by computing the Catalan number Catalan(r) and subtracting
 *            the sum of the first (k-1) Narayana numbers
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    if (n < 1u) return 0;          /* undefined for indices < 1 */

    /* find row r such that T(r,r) >= n > T(r-1,r-1) */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    idx_t prev = (r - 1) * r / 2;   /* total terms before row r */
    idx_t k = n - prev;             /* column within the row (1‑based) */

    seq_t cat = catalan(r);         /* full row sum */

    if (k == 1) return cat;         /* first element is the Catalan number */

    /* subtract the first (k-1) Narayana numbers */
    seq_t sum = (seq_t)0;
    for (idx_t i = 1; i < k; ++i) {
        seq_t a1 = binom(r, i);
        seq_t a2 = binom(r, i - 1);
        seq_t val = a1 * a2;
        val /= (seq_t)r;            /* exact division */
        sum += val;
    }
    return cat - sum;
}
