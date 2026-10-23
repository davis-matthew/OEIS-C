/*
 * A016052  a(1) = 3; for n >= 1, a(n+1) = a(n) + sum of its digits.
 * https://oeis.org/A016052
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 3280, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A016052
   Recurrence: a(1) = 3;  a(n+1) = a(n) + digit_sum(a(n))  (n ≥ 1)
*/
#include <stdlib.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* For n ≥ 1 the sequence is defined.
       If n == 0 we return 0 (undefined but harmless). */
    if (n == 0) return (seq_t)0;

    seq_t val = (seq_t)3;          /* a(1) */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i) {
        val += digit_sum(val);
    }
    return val;
}
