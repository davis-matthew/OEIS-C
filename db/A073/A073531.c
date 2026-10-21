/*
 * A073531  Number of n-digit positive integers with all digits distinct.
 * https://oeis.org/A073531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   a(n) = 9 * 9!/(10-n)! for 1 ≤ n ≤ 10, a(n) = 0 for n > 10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0 || n > 10) return (seq_t)0;
    seq_t res = (seq_t)9;                    /* first digit: 1–9 */
    for (idx_t i = 1; i < n; ++i) {           /* remaining n-1 digits */
        res *= (seq_t)(10 - i);               /* 10-i = 9,8,7,… */
    }
    return res;
}
