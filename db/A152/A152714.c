/*
 * A152714  Triangle read by rows: T(n,k) = 3^min(k, n-k).
 * https://oeis.org/A152714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 263, u32 n <= 923, u64 n <= 3443,
 *            u128 n <= 5150, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A152714:  Triangle read by rows: T(n,k)=3^min(k,n-k).
    Flattened row by row: a(n) = 3^min(k, r-k) where r satisfies
    r(r+1)/2 ≤ n < (r+1)(r+2)/2  and k = n – r(r+1)/2. */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find the row r containing n.  cum = # of elements up to row r. */
    idx_t r   = 0;
    idx_t cum = 1;          /* row 0 contributes 1 element */
    while (n >= cum) {
        ++r;
        cum += r + 1;
    }

    /* Number of elements before row r. */
    idx_t prefix = cum - (r + 1);
    idx_t k = n - prefix;               /* position within the row */

    /* Minimum distance to an edge of the row. */
    idx_t m = k < (r - k) ? k : (r - k);

    return pow3(m);                      /* 3^m */
}
