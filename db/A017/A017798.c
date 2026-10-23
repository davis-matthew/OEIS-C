/*
 * A017798  Binomial coefficients C(82,n).
 * https://oeis.org/A017798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = binomial(82, n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 7, u64 n <= 21, u128 n <= 82,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    const idx_t N = 82;
    static seq_t row[83];
    static unsigned char initialized = 0;

    if (!initialized) {
        /* initialise Pascal's triangle for row 82 using only addition */
        for (int i = 0; i <= N; ++i) row[i] = 0;
        row[0] = 1;
        initialized = 1;
        for (int i = 1; i <= N; ++i) {
            for (int k = i; k >= 1; --k) {
                row[k] = row[k] + row[k - 1];
            }
        }
    }

    return (n <= N) ? row[(int)n] : 0;
}
