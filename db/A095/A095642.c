/*
 * A095642  a(n) = 126 written in base n.
 * https://oeis.org/A095642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   repunit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 126 expressed as a number whose decimal digits are the
   digits of 126 in base n.  For n=1 the result is the 126‑digit
   repunit 111…1.  */
#include <stdint.h>
#include "../utils/repunit.h"

seq_t a(idx_t n)
{
    /* base 1: 126 ones */
    if (n == 1)
        return repunit(126);

    /* digits of 126 in base n (small number of digits) */
    uint64_t tmp = 126;
    uint64_t digits[10];
    int dlen = 0;
    while (tmp > 0) {
        digits[dlen++] = tmp % n;
        tmp /= n;
    }
    /* build the decimal number whose decimal digits are the above */
    seq_t result = 0;
    for (int i = dlen - 1; i >= 0; --i)
        result = result * 10 + (seq_t)digits[i];
    return result;
}
