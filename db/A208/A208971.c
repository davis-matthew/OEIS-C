/*
 * A208971  Number of 4-bead necklaces labeled with numbers -n..n allowing
 *          reversal, with sum zero and first and second differences in -n..n.
 * https://oeis.org/A208971
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^3 + 6*n^2 + 28*n + 48)/48 for even n a(n) = (2*n^3 +
 *            6*n^2 + 22*n + 18)/48 for odd n
 * Range:     u8 n <= 17, u16 n <= 115, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset begins at 1; 0 not defined */
    seq_t nn = (seq_t)n;             /* promote to seq_t first */

    if ((nn & 1) == 0) {            /* even n */
        return ((seq_t)2 * nn * nn * nn
                + (seq_t)6 * nn * nn
                + (seq_t)28 * nn
                + 48) / 48;
    } else {                        /* odd n */
        return ((seq_t)2 * nn * nn * nn
                + (seq_t)6 * nn * nn
                + (seq_t)22 * nn
                + 18) / 48;
    }
}
