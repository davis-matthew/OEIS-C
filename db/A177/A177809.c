/*
 * A177809  Symmetrical sequence:Binomial(n,5*m).
 * https://oeis.org/A177809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(5*r , 5*j) where r = largest integer with
 *            r*(r+1)/2 ≤ n, j = n - r*(r+1)/2
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 30, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Binomial helper supplied by the build environment */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    idx_t cumulative = 0;               /* terms before the current row */
    while (cumulative + r + 1 <= n) {
        cumulative += r + 1;
        ++r;
    }

    /* position j within the row */
    idx_t j = n - cumulative;           /* 0 ≤ j ≤ r */

    /* compute binomial(5*r , 5*j) */
    idx_t upper = 5 * r;
    idx_t lower = 5 * j;

    return binomial(upper, lower);
}
