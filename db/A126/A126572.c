/*
 * A126572  Array read by antidiagonals: a(n,m) = the m-th integer from among
 *          those positive integers coprime to n.
 * https://oeis.org/A126572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A126572 – Array read by antidiagonals
   a(n) = value at position n when the array a(i,j) (where a(i,j)
   is the j‑th integer coprime to i) is read along antidiagonals
   with the order (i decreasing, j increasing) within each
   diagonal. */
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* handle non‑positive indices if called erroneously */
    if (n == 0) { return 0; }

    /* Find diagonal d such that d*(d-1)/2 < n <= d*(d+1)/2   */
    idx_t d = 1;
    idx_t cumulative = 1;          /* total elements up to current diagonal */
    while (cumulative < n) {
        ++d;
        cumulative += d; /* cumulative = d*(d+1)/2 after the increment */
    }

    /* Position within diagonal (1 … d) */
    idx_t prev_cumulative = cumulative - d; /* sum up to d-1 */
    idx_t pos = n - prev_cumulative;        /* 1‑based position in diagonal */

    /* Corresponding row i and column j in the array */
    idx_t i = d - pos + 1;   /* row number (coprime set index)   */
    idx_t j = pos;           /* column number (ordinal in list)  */

    /* Find the j-th positive integer coprime to i */
    idx_t count = 0;
    seq_t result = 0;
    for (idx_t num = 1; ; ++num) {
        if (gcd((seq_t)i, (seq_t)num) == 1) {
            ++count;
            if (count == j) {
                result = (seq_t)num;
                break;
            }
        }
    }
    return result;
}
