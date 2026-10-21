/*
 * A222463  a(n) = n*5/gcd(n*5,n+5), n >= 5.
 * https://oeis.org/A222463
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*5/gcd(n*5,n+5) (with n>=5)
 * Range:     u8 n <= 51, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 5) {
        return 0;       /* values below the offset are undefined */
    }
    seq_t num = (seq_t)n * 5;
    seq_t g   = gcd(25, (seq_t)n + 5);
    return num / g;
}
