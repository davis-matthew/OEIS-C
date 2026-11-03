/*
 * A140961  Number of 3 X n {0,1}-matrices such that: (a) first and second row
 *          have a common 1, (b) second and third row have a common 1.
 * https://oeis.org/A140961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8^n - 2*6^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* exponent as seq_t */
    seq_t exp = (seq_t)n;

    /* compute the powers */
    seq_t p8 = ipow(8, exp);
    seq_t p6 = ipow(6, exp);
    seq_t p5 = ipow(5, exp);

    /* assemble the result */
    seq_t res = p8;
    res -= p6;   /* first subtraction of 6^n */
    res -= p6;   /* second subtraction of 6^n (total 2*6^n) */
    res += p5;   /* add 5^n */

    return res;
}
