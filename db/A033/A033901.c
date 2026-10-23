/*
 * A033901  Sort then Add, a(1)=21.
 * https://oeis.org/A033901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-1) + sorted_asc_digits(a(n-1)), a(1) = 21
 *            sorted_asc_digits(x) is the integer formed by arranging the
 *            decimal digits of x in ascending order (leading zeros are
 *            dropped)
 * Range:     u8 n <= 5, u16 n <= 19, u32 n <= 61, u64 n <= 326,
 *            u128 n <= 1000, big any n
 * Helpers:   sorted_asc_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sorted_asc_digits.h"

seq_t a(idx_t n) {
    /* a(1) = 21 */
    seq_t current = (seq_t)21;
    if (n <= (idx_t)1) {
        return current;
    }
    for (idx_t i = 2; i <= n; ++i) {
        seq_t add = sorted_asc_digits(current);
        current = current + add;
    }
    return current;
}
