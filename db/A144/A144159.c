/*
 * A144159  Eigentriangle, sum of row n terms = A006054(n+2).
 * https://oeis.org/A144159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = floor((r - c + 2)/2) * A006054(c) * where r = smallest
 *            integer with r(r+1)/2 >= n, * c = n - r(r-1)/2 (column index in
 *            row r). * A006054 is the rightmost element of each row of the
 *            triangle, * computed recursively from the triangle itself
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Find the row r that contains the n‑th term (1‑based). */
    idx_t r = 0;
    idx_t cumulative = 0;
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    /* Number of terms before row r (triangular number). */
    idx_t prev = (r - 1) * r / 2;
    idx_t c = n - prev;                /* column in this row (1‑based) */

    /* Cache to store A006054 values up to the current maximum row. */
    static seq_t *a006 = NULL;        /* 1‑based array, a006[1] = 1 */
    static size_t a006_len = 0;        /* max index computed so far */
    static size_t a006_cap = 0;        /* allocated capacity */

    if (a006_len < (size_t)r) {
        size_t needed = (size_t)r + 1;  /* we need indices up to r */
        if (needed > a006_cap) {
            seq_t *tmp = (seq_t *)realloc(a006, needed * sizeof(seq_t));
            if (!tmp)
                abort();
            a006 = tmp;
            a006_cap = needed;
        }
        /* Compute missing A006054 values. */
        for (size_t k = a006_len + 1; k <= r; ++k) {
            if (k == 1) {
                a006[1] = (seq_t)1;
            } else {
                size_t row = k - 1;  /* we need the sum of row (k-1) */
                seq_t sum = 0;
                for (size_t col = 1; col <= row; ++col) {
                    seq_t factor = (seq_t)((row - col + 2) / 2);
                    sum += factor * a006[col];
                }
                a006[k] = sum;   /* rightmost element of row (k-1) */
            }
        }
        a006_len = r;
    }

    /* Compute the requested term */
    seq_t factor = (seq_t)((r - c + 2) / 2);
    return factor * a006[c];
}
