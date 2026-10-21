/*
 * A051778  Triangle read by rows, where row (n) = n mod (n-1), n mod (n-2), n
 *          mod (n-3), ...n mod 2.
 * https://oeis.org/A051778
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   a(n) = r mod (r - (n - 2 - S(r-1))) where r = min{m >= 3 |
 *            (m-2)(m-1)/2 >= n-2} and S(t) = (t-2)(t-1)/2 is the number of
 *            terms in rows 3..t. The triangle is defined by rows: row(r) = [
 *            r mod (r-1), r mod (r-2), …, r mod 2 ]
 * Range:     u8 n <= 7628, u16 n <= 7628, u32 n <= 7628, u64 n <= 7628,
 *            u128 n <= 7628, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* offset is 3, so term indices start at n == 3 */
    if (n < 3) return 0;

    idx_t m = n - 2;            /* number of terms preceding this one in the
                                   flattened triangle */
    idx_t cumulative = 0;       /* terms covered in rows processed so far */
    idx_t r = 3;                /* current row number */

    /* Find row r such that cumulative + (r-2) >= m */
    while (cumulative + (r - 2) < m) {
        cumulative += (r - 2);
        ++r;
    }

    /* Position within the row (1‑based) */
    idx_t pos_in_row = m - cumulative;          /* 1 ≤ pos_in_row ≤ r-2 */

    /* Corresponding divisor j = r - pos_in_row   (j ranges from r-1 down to 2) */
    idx_t j = r - pos_in_row;

    return (seq_t)r % (seq_t)j;
}
