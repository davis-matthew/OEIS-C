/*
 * A206335  Number of (n+1)X(n+1) 0..3 arrays with every 2X2 subblock having
 *          nonzero determinant and commuting with every horizontal or
 *          vertical neighbor.
 * https://oeis.org/A206335
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 192, a(2) = 181, a(3) = 1340, a(4) = 1581, a(5) = 12021,
 *            a(n) = 9 * a(n-2) - 48 for n >= 6
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    if (n == 1) return 192;
    if (n == 2) return 181;
    if (n == 3) return 1340;
    if (n == 4) return 1581;
    if (n == 5) return 12021;

    /* Recurrence: a(n) = 9*a(n-2) - 48 for n >= 6.
       We evaluate it iteratively without extra storage. */

    if ((n & 1) == 0) {          /* even n : start from a4 */
        seq_t val = 1581;          /* a(4) */
        for (idx_t i = 4; i < n; i += 2)
            val = val * 9 - 48;    /* a(i+2) = 9*a(i) - 48 */
        return val;
    } else {                      /* odd n >= 7 : start from a5 */
        seq_t val = 12021;         /* a(5) */
        for (idx_t i = 5; i < n; i += 2)
            val = val * 9 - 48;    /* a(i+2) = 9*a(i) - 48 */
        return val;
    }
}
