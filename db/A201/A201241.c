/*
 * A201241  Number of ways to place 7 non-attacking wazirs on an n X n
 *          toroidal board.
 * https://oeis.org/A201241
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^2 * (n^12 - 105*n^10 + 4795*n^8 - 122115*n^6 +
 *            1834084*n^4 - 15461460*n^2 + 57441600) / 5040 for n >= 8; for n
 *            < 8 the values are taken from the OEIS table
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* small n table */
    static const seq_t small[8] = {
        0,          /* a(0) unused */
        0,          /* a(1) */
        0,          /* a(2) */
        0,          /* a(3) */
        16,         /* a(4) */
        4450,       /* a(5) */
        442980,     /* a(6) */
        11281312    /* a(7) */
    };

    if (n < 8) {
        return small[n];
    }

    /* compute powers of n */
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t n4 = nn * nn;                      /* n^4 */
    seq_t n6 = n4 * nn;                      /* n^6 */
    seq_t n8 = n4 * n4;                      /* n^8 */
    seq_t n10 = n8 * nn;                     /* n^10 */
    seq_t n12 = n8 * n4;                     /* n^12 */

    /* compute polynomial part */
    seq_t poly = n12;
    poly -= (seq_t)105 * n10;
    poly += (seq_t)4795 * n8;
    poly -= (seq_t)122115 * n6;
    poly += (seq_t)1834084 * n4;
    poly -= (seq_t)15461460 * nn;
    poly += (seq_t)57441600;

    /* multiply by n^2 and divide by 5040 */
    seq_t result = nn * poly;
    result /= (seq_t)5040;

    return result;
}
