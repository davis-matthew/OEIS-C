/*
 * A018593  Divisors of 688.
 * https://oeis.org/A018593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   a(n) = the n-th divisor of 688, sorted in increasing order
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t divs[10];
    static bool init = false;
    static const idx_t len = 10;

    if (!init) {
        idx_t idx = 0;
        for (idx_t j = 0; j <= 1; ++j) {             /* exponent of 43 */
            seq_t f43 = (j == 1) ? (seq_t)43 : (seq_t)1;
            seq_t p2 = (seq_t)1;
            for (idx_t k = 0; k <= 4; ++k) {          /* exponent of 2 (0..4) */
                divs[idx++] = p2 * f43;
                p2 <<= 1;                             /* multiply by 2 */
            }
        }
        init = true;
    }

    if (n >= 1 && n <= len)
        return divs[n - 1];
    return (seq_t)0;   /* out of bounds: no further divisors */
}
