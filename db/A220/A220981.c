/*
 * A220981  a(n) = 6^(4n+2) - 6^(3n+2) + 3 * 6^(2n+1) - 6^(n+1) + 1: the left
 *          Aurifeuillian factor of 6^(12n+6) + 1.
 * https://oeis.org/A220981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(4n+2) - 6^(3n+2) + 3 * 6^(2n+1) - 6^(n+1) + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 11 terms through n = 10 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    const seq_t six = 6;
    idx_t exp1 = 4 * n + 2;   /* 4n+2 */
    idx_t exp2 = 3 * n + 2;   /* 3n+2 */
    idx_t exp3 = 2 * n + 1;   /* 2n+1 */
    idx_t exp4 = n + 1;       /* n+1 */

    seq_t t1 = ipow(six, (seq_t)exp1);   /* 6^(4n+2) */
    seq_t t2 = ipow(six, (seq_t)exp2);   /* 6^(3n+2) */
    seq_t t3 = ipow(six, (seq_t)exp3);   /* 6^(2n+1) */
    seq_t t4 = ipow(six, (seq_t)exp4);   /* 6^(n+1) */

    seq_t result = t1;
    result -= t2;            /* subtract 6^(3n+2) */
    result += t3 * 3;        /* add 3 * 6^(2n+1) */
    result -= t4;            /* subtract 6^(n+1) */
    result += 1;             /* add the constant 1 */

    return result;
}
