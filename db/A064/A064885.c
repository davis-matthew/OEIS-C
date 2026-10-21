/*
 * A064885  Eisenstein array Ei(3,2).
 * https://oeis.org/A064885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   einstein_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Computes A064885 – Eisenstein array Ei(3,2).
 *
 * Recurrence:
 *   a(1,0) = 3, a(1,1) = 2
 *   For r > 1:
 *     if m is even: a(r,m) = a(r-1, m/2)
 *     else        : a(r,m) = a(r-1, (m-1)/2) + a(r-1, (m+1)/2)
 *
 * The sequence a(n) is the row‑major ordering of the triangle
 * {a(r,m)}.  Row r has 2^(r-1)+1 entries (m = 0 … 2^(r-1)).
 * The total number of terms up to row r is T(r) = (2^r - 1) + r.
 */

/* Include required standard headers */
#include <stdint.h>
#include "../utils/einstein_value.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n is 1‑based; n >= 1 */
    /* Find the row r that contains the n‑th term */
    idx_t r = 1;
    idx_t count = 0;           /* total count up to previous row */
    while (1) {
        /* Size of current row: 2^(r-1) + 1 */
        idx_t row_size = (1ULL << (r - 1)) + 1;
        if (count + row_size >= n) {
            break;           /* n lies in row r */
        }
        count += row_size;
        r++;
    }

    /* Column index within the found row */
    idx_t m = n - count - 1;  /* 0‑based index */

    /* Compute the value using the recurrence */
    return einstein_value(r, m);
}
