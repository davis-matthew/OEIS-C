/*
 * A115303  a(n) = n if n < 3, otherwise 3*a(floor(n/3)) + 2 - n mod 3.
 * https://oeis.org/A115303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n for n < 3 * a(n) = 3*a(floor(n/3)) + 2 - (n mod 3) for
 *            n ≥ 3 * Initial values: a(1)=1, a(2)=2
 * Range:     u8 n <= 242, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 3 the recurrence reduces to a(n) = n.      */
    if (n < 3) return (seq_t)n;

    /* Extract base‑3 digits of n (least significant first).  */
    unsigned char digits[64];
    int len = 0;
    idx_t temp = n;
    while (temp > 0) {
        digits[len++] = (unsigned char)(temp % 3);
        temp /= 3;
    }

    /* Reconstruct the result applying the digit transformation
       (all non‑leading digits d become 2‑d).                  */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        unsigned char d = digits[i];
        if (i != len - 1) d = 2 - d;   /* non‑leading digit */
        result = result * 3 + (seq_t)d;
    }
    return result;
}
