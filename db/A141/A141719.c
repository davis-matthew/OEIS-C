/*
 * A141719  Triangle online: 4, A140978(n+1) - A133819 .
 * https://oeis.org/A141719
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   * a(n) = 4*r if k == 1 or k == r * a(n) = r*(r+2) -
 *            (k-2)*((k-2)+2) otherwise * * where * r = smallest integer with
 *            n ≤ r*(r+1)/2 (row index) * k = n - (r-1)*r/2 (column index
 *            within the row) * * The triangle is: * row 1 : 4 * row 2 : 8, 8
 *            * row 3 : 12, 15, 12 * row 4 : 16, 24, 21, 16 * …
 * Range:     u8 n <= 56, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)  /* not used; offset is 1 */
        return (seq_t)0;

    /* Find the row r such that n <= T(r)=r*(r+1)/2 */
    idx_t r = 1;
    for (;; ++r) {
        /* Use 128‑bit intermediate to avoid overflow on large n */
        unsigned __int128 tri = (unsigned __int128)r * (r + 1) / 2;
        if (tri >= n)
            break;
    }

    /* Previous triangular number */
    unsigned __int128 tri_prev = (unsigned __int128)(r - 1) * r / 2;
    idx_t k = n - (idx_t)tri_prev;   /* column index within the row (1‑based) */

    if (k == 1 || k == r)
        return (seq_t)(4 * (seq_t)r);

    /* Interior terms:
     *   first interior value = r*(r+2)
     *   each subsequent value subtracts the next odd number 3,5,7,...
     *   after j subtractions the value is r*(r+2) – j*(j+2)
     */
    idx_t j = k - 2;                  /* number of subtractions performed */
    seq_t r_se = (seq_t)r;
    seq_t first = r_se * (r_se + 2);
    seq_t sub   = (seq_t)j * ((seq_t)j + 2);
    return first - sub;
}
