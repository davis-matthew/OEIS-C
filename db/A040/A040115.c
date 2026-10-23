/*
 * A040115  Concatenate absolute values of differences between adjacent digits
 *          of n.
 * https://oeis.org/A040115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = concatenate(|digits(i)-digits(i+1)|) for the decimal
 *            expansion of n, discarding leading zeros. For n < 10 the result
 *            is 0
 * Range:     u8 n <= 1379, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* n has at most 20 decimal digits (uint64_t max).  We store the digits
       in reverse order to avoid division by powers of 10. */
    if (n < 10u)
        return (seq_t)0;

    uint8_t buf[20];
    size_t len = 0;
    uint64_t tmp = n;
    while (tmp) {
        buf[len++] = (uint8_t)(tmp % 10u);
        tmp /= 10u;
    }

    seq_t result = 0;
    bool seen = false;  /* whether a non‑zero difference has been appended */

    /* Process digits from most significant to least significant. */
    for (size_t i = len; i > 1; --i) {
        int d1 = buf[i - 1];      /* more significant digit */
        int d2 = buf[i - 2];      /* less significant digit */
        int diff = d1 > d2 ? d1 - d2 : d2 - d1;  /* |d1-d2| */

        if (!seen) {
            if (diff == 0)          /* skip leading zeros */
                continue;
            seen = true;
        }
        result = result * (seq_t)10 + (seq_t)diff;
    }
    return result;
}
