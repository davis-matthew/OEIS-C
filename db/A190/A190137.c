/*
 * A190137  Least k in {1,2,3,4,5,6,7,8,9} such that all decimal digits of k*n
 *          are less than or equal to k.
 * https://oeis.org/A190137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least k in {1..9} such that all digits of k*n are <= k
 * Range:     u8 n <= 51, u16 n <= 8712, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/get_digits.h"  // assumed to provide: void get_digits(seq_t, uint8_t*, size_t*)

static const size_t MAX_DIGITS = 256;  // sufficient for typical n, can be increased if needed

seq_t a(idx_t n)
{
    /* n >= 1 by definition of OEIS offset */
    for (int k = 1; k <= 9; ++k) {
        seq_t prod = (seq_t)k * (seq_t)n;

        uint8_t buf[MAX_DIGITS];
        size_t len;
        get_digits(prod, buf, &len);

        bool ok = true;
        for (size_t i = 0; i < len; ++i) {
            if (buf[i] > (uint8_t)k) {  // digit greater than k
                ok = false;
                break;
            }
        }
        if (ok) return (seq_t)k;   // first k that satisfies the condition
    }
    /* Should never reach here because k = 9 always works */
    return (seq_t)9;
}
