/*
 * A125947  a(n) is the number of nonnegative integers k less than 10^n such
 *          that the decimal representation of k lacks the digit 1, at least
 *          one of digits 2,3,4,5 and at least one of digits 6,7,8,9.
 * https://oeis.org/A125947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 16*7^n-48*6^n+68*5^n-56*4^n+28*3^n-8*2^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 872 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    seq_t p7 = ipow((seq_t)7, (seq_t)n);
    seq_t p6 = ipow((seq_t)6, (seq_t)n);
    seq_t p5 = ipow((seq_t)5, (seq_t)n);
    seq_t p4 = ipow((seq_t)4, (seq_t)n);
    seq_t p3 = ipow((seq_t)3, (seq_t)n);
    seq_t p2 = ipow((seq_t)2, (seq_t)n);

    seq_t res = (seq_t)16 * p7;
    res -= (seq_t)48 * p6;
    res += (seq_t)68 * p5;
    res -= (seq_t)56 * p4;
    res += (seq_t)28 * p3;
    res -= (seq_t)8 * p2;
    res += (seq_t)1;
    return res;
}
