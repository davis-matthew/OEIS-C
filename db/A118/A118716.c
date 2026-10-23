/*
 * A118716  Number of different ways to represent n in different bases using
 *          only decimal digits or characters (0 to 9).
 * https://oeis.org/A118716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = #{b : 1 <= b <= n+1, all digits of n in base b are <= 9},
 *            with the special case a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Handle the exceptional case of n == 1. */
    if (n == 1) return 1;

    /* Base 1 (unary) always counts. */
    seq_t count = 1;

    /* Count bases 2 through n. */
    for (idx_t b = 2; b <= n; ++b) {
        idx_t m = n;
        bool ok = true;
        while (m > 0) {
            idx_t digit = m % b;
            if (digit >= 10) {       /* digit too large for decimal */
                ok = false;
                break;
            }
            m /= b;
        }
        if (ok) ++count;
    }

    /* Base n+1 adds a single decimal digit n if n <= 9. */
    if (n <= 9) ++count;

    return count;
}
