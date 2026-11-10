/*
 * A230287  First differences of A016052/3 (= A230286).
 * https://oeis.org/A230287
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = digit_sum(b(n))/3 where b(1) = 3, b(k+1) = b(k) +
 *            digit_sum(b(k)) (A016052); equals (b(n+1)-b(n))/3
 * Range:     u8 n <= 32, u16 n <= 3760, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 119 terms through n = 9157 (2026-11-07)
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    seq_t val = (seq_t)3;
    for (idx_t i = 2; i <= n; ++i) {
        val += digit_sum(val);
    }
    return digit_sum(val) / 3;
}
