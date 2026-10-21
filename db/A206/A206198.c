/*
 * A206198  Number of (n+1) X (n+1) 0..3 arrays with every 2 X 3 or 3 X 2
 *          subblock having exactly one clockwise edge increases.
 * https://oeis.org/A206198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=256, a(2)=1060, a(3)=5136, a(4)=32920, a(5)=118936,
 *            a(6)=484704, a(7)=3270240; a(n) = 100*a(n-3) for n > 7
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* initial values up to n = 7 */
    static const seq_t init[8] = {
        0,          /* dummy for index 0 */
        256,        /* a(1) */
        1060,       /* a(2) */
        5136,       /* a(3) */
        32920,      /* a(4) */
        118936,     /* a(5) */
        484704,     /* a(6) */
        3270240     /* a(7) */
    };

    if (n <= 7) {
        return init[n];
    }

    /* For n > 7 use recurrence a(n) = 100*a(n-3) */
    idx_t offset = (n - 5) % 3;      /* 0 -> a(5), 1 -> a(6), 2 -> a(7) */
    idx_t power  = (n - 5) / 3;      /* number of times to multiply by 100 */

    seq_t result = init[5 + offset];
    for (idx_t i = 0; i < power; ++i) {
        result = result * 100u;     /* multiply by 100 (modulo arithmetic on unsigned types) */
    }
    return result;
}
