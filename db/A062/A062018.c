/*
 * A062018  a(n) = n^n written backwards.
 * https://oeis.org/A062018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n^n written backwards
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   pow_uint, reverse_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/reverse_digits.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined for n=0, return 0 safely */

    /* Count factors of 2 and 5 in n */
    idx_t v2 = 0, v5 = 0, tmp = n;
    while ((tmp & 1u) == 0 && tmp > 0) { v2++; tmp >>= 1; }
    tmp = n;
    while (tmp % 5u == 0 && tmp > 0) { v5++; tmp /= 5; }

    /* Compute m = n / (2^v2 * 5^v5)  (coprime with 10) */
    idx_t m = n;
    for (idx_t i = 0; i < v2; ++i) m /= 2;
    for (idx_t i = 0; i < v5; ++i) m /= 5;

    /* Base value: m^n */
    seq_t val = pow_uint((uint64_t)m, n);

    /* Multiply by the remaining power of 2 or 5 after removing trailing zeros */
    if (v2 != v5) {
        idx_t diff = n * ( (v2 > v5) ? (v2 - v5) : (v5 - v2) );
        uint64_t base = (v2 > v5) ? 2 : 5;
        seq_t mult = pow_uint(base, diff);
        val *= mult;
    }

    /* Return the decimal digits of the result reversed */
    return reverse_digits(val);
}
