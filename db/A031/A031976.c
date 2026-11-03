/*
 * A031976  Remove digits 0, 1, 4, 9 from n.
 * https://oeis.org/A031976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A031976  
   a(n) = the n-th positive integer obtained by deleting all decimal
   digits 0, 1, 4, 9 from successive natural numbers starting at 1.  */
#include <stdint.h>

static seq_t strip_forbidden(uint64_t x)
{
    /* Extract decimal digits of x into an array in reverse order. */
    int digits[20];
    int len = 0;
    if (x == 0) {
        digits[len++] = 0;
    } else {
        while (x > 0) {
            digits[len++] = (int)(x % 10);
            x /= 10;
        }
    }

    /* Build the number after removing forbidden digits. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        if (d == 0 || d == 1 || d == 4 || d == 9) {
            continue;           /* skip forbidden digits */
        }
        result = result * 10 + d;
    }
    return result;   /* empty -> 0 */
}

seq_t a(idx_t n)
{
    idx_t candidate = 1;
    idx_t count = 0;
    for (;;) {
        seq_t val = strip_forbidden((uint64_t)candidate);
        if (val != 0) {
            ++count;
            if (count == n) {
                return val;
            }
        }
        ++candidate;
    }
}
