/*
 * A143083  A triangle of coefficients: T(n,m) = (2*n + 2*m + 3)! / (2*(2*m +
 *          1)!*(2*n + 1)!).
 * https://oeis.org/A143083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 35, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A143083: Triangle of coefficients
 * Formula: a(n) = binom(2*r+2*c+3, 2*r+1)*(c+1)
 * where r = floor((sqrt(8*n-7)-1)/2) is the 0‑based row number
 * and c = n - r*(r+1)/2 - 1 is the 0‑based column number.
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for n < 1 */

    /* Find the smallest r such that r*(r+1)/2 >= n.
     * r will be the 1‑based row index + 1.            */
    idx_t low = 0;
    idx_t high = n;                /* high satisfies the inequality */

    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        if (mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid;
    }
    idx_t r = high;                /* 1‑based row index */
    idx_t prev = (r - 1) * r / 2;  /* number of terms before this row */
    idx_t c1 = n - prev;           /* 1‑based column index in this row */
    idx_t row0 = r - 1;            /* convert to 0‑based indices */
    idx_t col0 = c1 - 1;
    idx_t numerator = 2 * row0 + 2 * col0 + 3;
    idx_t k = 2 * row0 + 1;

    seq_t bin = binom(numerator, k);
    return bin * (seq_t)(col0 + 1);
}
