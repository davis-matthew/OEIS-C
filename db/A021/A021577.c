/*
 * A021577  Decimal expansion of 1/573.
 * https://oeis.org/A021577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n-th digit (after the decimal point) of the decimal
 *            expansion of 1/573. The sequence repeats with period equal to
 *            the multiplicative order of 10 modulo 573, which is ≤ 572
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    static seq_t digits[600];
    static size_t period = 0;

    if (period == 0) {                 /* compute digits once */
        uint64_t rem = 1;               /* initial remainder for 1/573 */
        size_t idx = 0;

        do {
            rem *= 10;
            digits[idx++] = (seq_t)(rem / 573);
            rem %= 573;
        } while (rem != 1);             /* stop when the remainder repeats */

        period = idx;                   /* period length */
    }

    return digits[(size_t)(n % period)];
}
