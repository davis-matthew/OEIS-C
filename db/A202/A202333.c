/*
 * A202333  Number of (n+1) X 7 binary arrays with consecutive windows of two
 *          bits considered as a binary number nondecreasing in every row and
 *          column.
 * https://oeis.org/A202333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^7 + 77*n^6 + 1169*n^5 + 9275*n^4 + 41993*n^3 +
 *            109088*n^2 + 150876*n + 95760) / 5040
 * Range:     u8 none, u16 none, u32 n <= 16, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the polynomial provided in the OEIS entry. */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* not used in the sequence, but safe */

    seq_t ni   = (seq_t)n;        /* n */
    seq_t n2   = ni * ni;         /* n^2 */
    seq_t n3   = n2 * ni;         /* n^3 */
    seq_t n4   = n3 * ni;         /* n^4 */
    seq_t n5   = n4 * ni;         /* n^5 */
    seq_t n6   = n5 * ni;         /* n^6 */
    seq_t n7   = n6 * ni;         /* n^7 */

    seq_t num = 2 * n7
              + 77 * n6
              + 1169 * n5
              + 9275 * n4
              + 41993 * n3
              + 109088 * n2
              + 150876 * ni
              + 95760;

    return num / 5040;
}
