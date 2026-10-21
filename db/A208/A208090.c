/*
 * A208090  100 written in base -n.
 * https://oeis.org/A208090
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, fini, full, less
 * Formula:   a(n) = digits of 100 in base –n concatenated as decimal digits
 * Range:     u8 none, u16 none, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    assert(n >= 2);          /* OEIS offset starts at 2 */

    /* digits[r] will hold the least‑significant base –n digit first */
    int digits[64];
    int k = 0;
    int64_t x = 100;         /* decimal value to convert */

    /* Convert x to base –n */
    while (x != 0) {
        int64_t r = x % (int64_t)n;   /* r may be negative */
        if (r < 0)
            r += n;                  /* make remainder non‑negative */
        digits[k++] = (int)r;
        x = -(x - r) / (int64_t)n;   /* next quotient */
    }

    /* Build the concatenated decimal integer from most‑significant digit */
    seq_t result = 0;
    for (int i = k - 1; i >= 0; --i) {
        int d = digits[i];

        /* Determine how many decimal digits the base‑n digit has */
        int len = 0;
        int64_t tmp = d;
        do {
            ++len;
            tmp /= 10;
        } while (tmp);

        result = result * pow10((idx_t)len) + (seq_t)d;
    }

    return result;
}
