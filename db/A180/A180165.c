/*
 * A180165  Triangle read by rows, derived from an array of sequences
 *          generated from (1 + x)/ (1 - r*x - r*x^2).
 * https://oeis.org/A180165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th term of the triangle read by rows described in
 *            OEIS A180165. For a given n find the row r such that r(r+1)/2 ≥
 *            n and (r−1)r/2 < n. Let k be the position inside that row: k = n
 *            − (r−1)r/2 (1 ≤ k ≤ r). Then a(n) = f_s(k−1) where s = r − k + 1
 *            and f_s is the sequence defined by f_s(...
 * Range:     u8 n <= 32, u16 n <= 72, u32 n <= 164, u64 n <= 424,
 *            u128 n <= 1114, big any n
 * Helpers:   f_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/f_seq.h"

seq_t a(idx_t n)
{
    /* find minimal r such that r(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = (idx_t)1;          /* current r(r+1)/2, starting with r=1 */
    while (tri < n) {
        ++r;
        tri += r;                  /* tri now = r(r+1)/2 */
    }

    /* position k within the row (1‑based) */
    idx_t k = n - (tri - r);      /* since (r-1)r/2 = tri - r */

    /* compute the underlying sequence index */
    idx_t s = r - k + 1;
    idx_t m = k - 1;              /* 0‑based index inside f_s */

    return f_seq(s, m);
}
