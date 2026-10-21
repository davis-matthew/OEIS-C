/*
 * A094531  Array read by rows: right-hand side of triangle A027907 of
 *          trinomial coefficients.
 * https://oeis.org/A094531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) is the coefficient of x^(e) in the trinomial expansion
 * (1 + x + x^2)^r, where r is the unique row index satisfying
 *       r*(r+1)/2  ≤  n  <  (r+1)*(r+2)/2
 * and e = r + (n - r*(r+1)/2).  This copies the right‑hand side of
 * the trinomial triangle A027907 row by row.
 */

#include "../utils/binom.h"      /* seq_t binom(idx_t a, idx_t b); */

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    for (;; ++r) {
        unsigned __int128 val = (unsigned __int128)r * (r + 1) / 2;
        if (val > n) {
            --r;
            break;
        }
    }

    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t offset = n - start;          /* 0 <= offset <= r */
    idx_t e = r + offset;              /* exponent to extract */

    /* Compute the trinomial coefficient:
     * coefficient of x^e = sum_{j=ceil(e/2)}^{min(e,r)} C(r,j) * C(j, e - j)   */
    idx_t j_min = (e + 1) / 2;                     /* ceil(e/2) */
    idx_t j_max = (e < r) ? e : r;                 /* min(e, r) */

    seq_t res = 0;
    for (idx_t j = j_min; j <= j_max; ++j) {
        seq_t t1 = binom(r, j);
        seq_t t2 = binom(j, e - j);
        res += t1 * t2;
    }
    return res;
}
