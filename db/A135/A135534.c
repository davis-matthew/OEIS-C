/*
 * A135534  a(1) = 1; for n>=1, a(2n) = A135561(n), a(2n+1) = 0.
 * https://oeis.org/A135534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A135534 a(1) = 1; for n >= 1, a(2n) = A135561(n), a(2n+1) = 0
 *            where A135561(1) = 3; A135561(m) = 1 if m is odd > 1; A135561(m)
 *            = 2^{v+2} - 1 for even m with odd part 1 (m = 2^{v}); A135561(m)
 *            = 2^{v+1} - 1 for other even m (odd part > 1), v = exponent of
 *            the largest power of 2 dividing m
 * Range:     u8 n <= 255, u16 n <= 65535, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 118 terms through n = 8730 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t A135561(idx_t m)
{
    if (m == 1)           /* special case for n = 1 */
        return (seq_t)3;

    if (m & 1)            /* odd m > 1 */
        return (seq_t)1;

    /* m is even: find v = v2(m) and the odd part */
    idx_t v = 0;
    idx_t odd = m;
    while ((odd & 1) == 0) {
        odd >>= 1;
        ++v;
    }

    seq_t one = (seq_t)1;
    if (odd == 1)          /* m = 2^v   */
        return (one << (v + 2)) - (seq_t)1;

    return (one << (v + 1)) - (seq_t)1;   /* odd part > 1 */
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (n & 1)            /* odd index > 1 */
        return (seq_t)0;

    return A135561(n >> 1);   /* n = 2 * m, m = n/2 */
}
