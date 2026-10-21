/*
 * A090369  Smallest divisor of 2n that is > 2, or 0 if no such divisor
 *          exists.
 * https://oeis.org/A090369
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest divisor of 2n that is > 2, or 0 if none
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* External types: seq_t and idx_t supplied by the build. */
seq_t a(idx_t n) {
    if (n == 1) return 0;               /* 2n = 2 has no divisor > 2 */

    seq_t m = (seq_t)n * 2;             /* m = 2n, guaranteed even */
    unsigned e = 0;                     /* number of trailing 2's */

    /* Strip all factors of 2 from m */
    while ((m & 1) == 0) {
        m >>= 1;
        ++e;
    }

    if (e >= 2) {                       /* m is divisible by 4 */
        if (m % 3 == 0) return 3;       /* 3 < 4 and divides m */
        return 4;                       /* otherwise smallest >2 divisor is 4 */
    }

    /* e == 1 : m is odd (the odd part of 2n) */
    /* Find the smallest odd divisor of m (>1) */
    for (seq_t d = 3; d <= m / d; d += 2) {
        if (m % d == 0) return d;      /* d divides m */
    }

    /* m itself is the smallest odd divisor */
    return m;
}
