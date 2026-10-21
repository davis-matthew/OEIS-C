/*
 * A091915  Maximum of even products of partitions of n.
 * https://oeis.org/A091915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 0, a(2) = 2, a(3) = 2 * * For n > 3: * if n % 3 == 0:
 *            a(n) = 3^(n/3 - 2) * 8 * if n % 3 == 1: a(n) = 3^((n-4)/3) * 4 *
 *            if n % 3 == 2: a(n) = 3^((n-2)/3) * 2 * This follows from the
 *            maximum‑product partition with at least one * part equal to 2.
 *            For n % 3 == 0 the best odd product (3^k) is...
 * Range:     u8 n <= 15, u16 n <= 30, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Special small values */
    if (n == 1) return 0;
    if (n == 2) return 2;
    if (n == 3) return 2;

    idx_t r = n % 3;
    seq_t res = 0;
    idx_t exp = 0;

    if (r == 0) {
        /* n = 3k, k >= 2 */
        exp = n / 3 - 2;           /* exponent of 3 */
        res = 8;                   /* 3^0 * 8 */
    } else if (r == 1) {
        /* n = 3k + 1, k >= 1 */
        exp = (n - 4) / 3;         /* exponent of 3 */
        res = 4;                   /* 4 * 3^0 */
    } else /* r == 2 */ {
        /* n = 3k + 2, k >= 1 */
        exp = (n - 2) / 3;         /* exponent of 3 */
        res = 2;                   /* 2 * 3^0 */
    }

    /* multiply by 3^exp */
    for (idx_t i = 0; i < exp; ++i) {
        res *= 3;
    }

    return res;
}
