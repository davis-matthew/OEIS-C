/*
 * A098752  a(1) = 1 and a(n+1) is the least number > a(n) that begins with
 *          the last digit of a(n) and doesn't end with 0.
 * https://oeis.org/A098752
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1) = 1; for n > 1: if n % 3 == 0: a(n) = 10^(n/3) + 2; if n %
 *            3 == 1: a(n) = 2 * 10^((n-1)/3) + 1; if n % 3 == 2: a(n) =
 *            10^((n+1)/3) + 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 28, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n) {
    if (n == 1)
        return (seq_t)1;

    idx_t r = n % 3;

    if (r == 0) {
        seq_t p = pow_uint(10ULL, n / 3);
        return p + (seq_t)2;
    } else if (r == 1) {
        seq_t p = pow_uint(10ULL, (n - 1) / 3);
        return (p << 1) + (seq_t)1; /* multiply by 2 and add 1 */
    } else { /* r == 2 */
        seq_t p = pow_uint(10ULL, (n + 1) / 3);
        return p + (seq_t)1;
    }
}
