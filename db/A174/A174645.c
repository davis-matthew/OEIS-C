/*
 * A174645  Number of ways to place 6 nonattacking amazons (superqueens) on a
 *          6 X n board.
 * https://oeis.org/A174645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * For n >= 17 the sequence is given by the degree‑6 polynomial *
 *            * a(n) = n^6 - 63*n^5 + 1775*n^4 - 28613*n^3 * + 277462*n^2 -
 *            1526716*n + 3699966 * * For 1 <= n < 17 the value is computed by
 *            exhaustive enumeration * of all 6‑column selections on a 6×n
 *            board and all ways to assign * the 6 rows to...
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 50, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   brute_small, recurse_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/brute_small.h"
#include "../utils/recurse_perm.h"

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n >= 17) {
        /* Compute the polynomial using Horner's rule. */
        seq_t n_seq   = (seq_t)n;
        seq_t n_sq    = n_seq * n_seq;          /* n^2  */
        seq_t n_cb    = n_sq * n_seq;           /* n^3  */
        seq_t n_4     = n_cb * n_seq;           /* n^4  */
        seq_t n_5     = n_4  * n_seq;           /* n^5  */
        seq_t n_6     = n_5  * n_seq;           /* n^6  */

        seq_t res = n_6;
        res -= (seq_t)63 * n_5;
        res += (seq_t)1775 * n_4;
        res -= (seq_t)28613 * n_cb;
        res += (seq_t)277462 * n_sq;
        res -= (seq_t)1526716 * n_seq;
        res += (seq_t)3699966;
        return res;
    } else {
        return brute_small(n);
    }
}
