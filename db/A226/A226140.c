/*
 * A226140  a(n) = Sum_{i=1..floor(n/2)} (n-i)^i.
 * https://oeis.org/A226140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} (n-i)^i
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 31, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 703 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    seq_t sum = 0;
    idx_t half = n >> 1;           /* floor(n/2) */

    for (idx_t i = 1; i <= half; ++i)
    {
        seq_t base  = (seq_t)(n - i);
        seq_t exp   = (seq_t)i;
        seq_t term  = ipow(base, exp);
        sum += term;
    }

    return sum;
}
