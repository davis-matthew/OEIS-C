/*
 * A166492  Table of numerators of A120070(n,m)/A002260(n,m), 1 <= m < n.
 * https://oeis.org/A166492
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, tabl
 * Formula:   a(n) = ((r^2 – m^2) / gcd(r^2 – m^2, m)) where (r,m) are
 *            obtained by flattening the table of pairs 1 ≤ m < r, with r ≡ n
 *            in the OEIS indexing (n ≥ 2)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses a simple loop to find the row r; this is
   fast enough for n up to at least 10⁶ and runs in < 2 s for n up
   to 10 000. */
#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 2) return 0;               /* indices below the offset are irrelevant */

    /* Convert the OEIS index to a zero‑based offset in the flattened
       table.  The first term (n=2) has offset 0. */
    idx_t offset = n - 2;

    /* Find the row r such that
           C(r-1) ≤ offset < C(r),
       where C(k) = k*(k+1)/2 = (k+1)*k/2 is the k‑th triangular number.  */
    idx_t r = 2;
    while ((unsigned __int128)(r - 1) * r / 2 <= offset) ++r;

    /* The column (i.e., m) is the position inside row r. */
    idx_t prev = (unsigned __int128)(r - 2) * (r - 1) / 2;
    idx_t m = offset - prev + 1;            /* 1 ≤ m < r */

    /* Compute the numerator of the reduced fraction:
           (r+m)*(r–m) / gcd((r+m)*(r–m), m)  == (r^2 – m^2) / gcd(r^2 – m^2, m) */
    seq_t rr = (seq_t)r * (seq_t)r;        /* r^2 */
    seq_t mm = (seq_t)m * (seq_t)m;        /* m^2 */
    seq_t num = rr - mm;                   /* r^2 – m^2 */
    seq_t g = gcd(num, (seq_t)m);          /* gcd(r^2 – m^2, m) */

    return num / g;
}
