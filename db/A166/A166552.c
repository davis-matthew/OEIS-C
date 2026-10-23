/*
 * A166552  a(n) = 3*a(n-2) for n > 2; a(1) = 1; a(2) = 4.
 * https://oeis.org/A166552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 81, u128 n <= 161,
 *            big any n
 * Helpers:   pow_3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 3*a(n-2) for n > 2; a(1) = 1; a(2) = 4
 * The sequence alternates between powers of 3 and four times powers of 3:
 *   a(odd n)  = 3^((n-1)/2)
 *   a(even n) = 4 * 3^((n-2)/2)
 */
#include <stdbool.h>
#include "../utils/pow_3.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    if (n & 1) {                 /* odd n */
        idx_t e = (n - 1) / 2;
        return pow_3(e);
    } else {                     /* even n */
        idx_t e = (n - 2) / 2;
        return pow_3(e) * (seq_t)4;
    }
}
