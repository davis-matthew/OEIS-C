/*
 * A061762  a(n) = (sum of digits of n) + (product of digits of n).
 * https://oeis.org/A061762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = sum of decimal digits of n + product of decimal digits of
 *            n
 * Range:     u8 n <= 398, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 0  (a(0) = 0) */
#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Special‑case n = 0:  sum = 0, product = 0 → result 0 */
    if (n == 0)
        return 0;

    idx_t t = n;
    seq_t sum = 0;
    seq_t prod = 1;

    /* Iterate over decimal digits of n */
    while (t) {
        /* Extract least significant decimal digit */
        idx_t digit = t % 10;
        sum += digit;      /* sum of digits (seq_t + digit) */
        prod *= digit;     /* product of digits (seq_t * digit) */
        t /= 10;
    }

    return sum + prod;
}
