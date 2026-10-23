/*
 * A123028  a(n) = (3*n^2 + 3*n + 1)*a(n-2), for n>2, with a(0) = a(1) = 1.
 * https://oeis.org/A123028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^2 + 3*n + 1) * a(n-2) (n > 2) a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;                 /* start with a(0) or a(1) */

    if ((n & 1) == 0) {                      /* even indices 0,2,4,… */
        for (idx_t i = 2; i <= n; i += 2) {
            seq_t factor = (seq_t)3 * (seq_t)i * (seq_t)i
                           + (seq_t)3 * (seq_t)i
                           + (seq_t)1;
            result = result * factor;
        }
    } else {                                 /* odd indices 1,3,5,… */
        for (idx_t i = 3; i <= n; i += 2) {
            seq_t factor = (seq_t)3 * (seq_t)i * (seq_t)i
                           + (seq_t)3 * (seq_t)i
                           + (seq_t)1;
            result = result * factor;
        }
    }

    return result;
}
