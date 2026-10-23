/*
 * A134444  (A000012 * A128174 + A128174 * A000012) - A000012.
 * https://oeis.org/A134444
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (A000012 * A128174 + A128174 * A000012) - A000012
 * Range:     u8 n <= 54, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is obtained by writing a lower‑triangular triangle of odd
   numbers.  For row r (0‑based) the elements are in decreasing odd
   order; the largest odd appears once when r is even and twice when r
   is odd, all smaller odd numbers appear twice.  The sequence is the
   row‑wise concatenation of this triangle. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r in which index n occurs.  The total number of
       terms up to row r is T(r) = (r+1)(r+2)/2.  We incrementally add
       row lengths until the cumulative total exceeds n. */
    idx_t r = 0;
    idx_t total = 1;              /* T(0) = 1 */
    while (total <= n) {
        ++r;
        total += r + 1;           /* add next row length r+1 */
    }

    /* total now equals T(r); previous total is T(r-1). */
    idx_t prev_total = total - (r + 1);
    idx_t offset     = n - prev_total;   /* 0‑based position inside row */

    /* Largest odd number in this row. */
    idx_t max_odd = 2 * (r / 2) + 1;   /* floor(r/2) = r/2 for integer division */

    if ((r & 1) == 0) {               /* r even: first odd occurs once */
        if (offset == 0)
            return (seq_t)max_odd;

        /* Skip the first element and process groups of two. */
        offset -= 1;
        idx_t group = offset / 2;     /* group index after the first odd */
        seq_t odd = (seq_t)(max_odd - 2 - 2 * group);
        return odd;
    } else {                          /* r odd: all odds occur twice */
        idx_t group = offset / 2;
        seq_t odd = (seq_t)(max_odd - 2 * group);
        return odd;
    }
}
