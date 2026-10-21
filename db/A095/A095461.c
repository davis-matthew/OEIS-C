/*
 * A095461  a(n) = 34 written in base 11 - n.
 * https://oeis.org/A095461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = "34" written in base (11 - n)
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   unary_34
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/unary_34.h"

seq_t a(idx_t n)
{
    /* compute the base value = 11 - n, signed to allow negative bases */
    int64_t b = 11 - (int64_t)n;

    /* For base 1: number is unary (34 ones) */
    if (b == 1)
        return unary_34();

    /* For base 0 or -1 the ordinary algorithm fails; return 0 */
    if (b == 0 || b == -1)
        return (seq_t)0;

    /* Use digits storage; the number of digits is tiny (≤ 64) */
    unsigned digits[64];
    unsigned len = 0;

    /* Positive base (b ≥ 2) */
    if (b >= 2) {
        uint64_t absB = (uint64_t)b;
        uint64_t temp = 34ULL;
        if (temp == 0)
            return 0;
        while (temp > 0) {
            unsigned d = (unsigned)(temp % absB);
            temp /= absB;
            digits[len++] = d;
        }
    }
    /* Negative base (b ≤ -2) */
    else /* b <= -2 */ {
        int64_t absB = -(int64_t)b;          /* positive absolute value of base */
        int64_t temp = 34LL;
        while (temp != 0) {
            int64_t r = temp % absB;         /* remainder may be negative */
            temp = temp / b;                 /* division rounds toward zero */
            if (r < 0) {                     /* make remainder positive */
                r += absB;
                temp += 1;
            }
            digits[len++] = (unsigned)r;
        }
    }

    /* Convert the digit array (most significant first) to a decimal integer */
    seq_t res = 0;
    for (int i = (int)len - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
