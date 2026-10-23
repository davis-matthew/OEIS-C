/*
 * A163284  Triangle read by rows in which row n lists n+1 terms, starting
 *          with n^4 and ending with n^5, such that the difference between
 *          successive terms is equal to n^4 - n^3.
 * https://oeis.org/A163284
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163284: row‑by‑row triangle
   The triangle has rows indexed by n.  Row n has n+1 terms:
   t(i) = n^4 + i·(n^4 – n^3)   (0 ≤ i ≤ n)
   The sequence a(n) is the triangle read by rows, so that
   a(0) = 0^4,
            a(1) = 1^4,
            a(2) = 1^4,
            a(3) = 2^4,
            a(4) = 2^4+ (2^4-2^3),
            ...
   The index n of a(n) is a linear offset; the function converts it
   to the corresponding row r and position i in that row and
   evaluates the closed form above. */

// note: seq_t must support multiplication and overflow‑safe
// arithmetic (e.g. uint128_t or big integer classes).

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       We binary‑search on r ∈ [0, n]. */
    idx_t low = 0;
    idx_t high = n;          // r cannot exceed n
    idx_t r = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        seq_t t = (seq_t)mid * ((seq_t)mid + 1) / 2; /* triangular number */
        if (t <= (seq_t)n) {
            r = mid;            /* mid is a candidate */
            low = mid + 1;      /* try larger r */
        } else {
            high = mid - 1;     /* try smaller r */
        }
    }

    /* offset of the first entry in row r */
    idx_t offset = (idx_t)(((seq_t)r * ((seq_t)r + 1)) / 2);
    idx_t i = n - offset;               /* position within the row */

    /* compute n^4, n^3, difference d and the term */
    seq_t N = (seq_t)r;
    seq_t N2 = N * N;              /* n^2 */
    seq_t N3 = N2 * N;             /* n^3 */
    seq_t N4 = N3 * N;             /* n^4 */
    seq_t d = N4 - N3;             /* n^4 - n^3 */
    seq_t term = N4 + ((seq_t)i) * d;   /* t(i) */

    return term;
}
