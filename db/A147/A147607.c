/*
 * A147607  Expansion of g.f.: 1/((1 - 2*x^2 + x^4 + 2*x^6 - x^8)*(1 - 2*x^2 -
 *          x^4 + 2*x^6 - x^8)).
 * https://oeis.org/A147607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 4*a(n-2) - 4*a(n-4) - 4*a(n-6) + 11*a(n-8) - 4*a(n-10) -
 *            4*a(n-12) + 4*a(n-14) - a(n-16); Initial values: a(0)=1, a(1)=0,
 *            a(2)=4, a(3)=0, a(4)=12, a(5)=0, a(6)=28, a(7)=0, a(8)=59,
 *            a(9)=0, a(10)=116, a(11)=0, a(12)=228, a(13)=0, a(14)=460,
 *            a(15)=0
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 57, u64 n <= 115,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* first 16 terms */
    static const seq_t init[16] = {
        1, 0, 4, 0, 12, 0, 28, 0,
        59, 0, 116, 0, 228, 0, 460, 0
    };

    if (n < 16) return init[(size_t)n];

    /* small buffer holding the last 16 terms computed */
    seq_t buf[16];
    for (size_t i = 0; i < 16; ++i) buf[i] = init[i];

    /* compute up to n */
    for (idx_t i = 16; i <= n; ++i) {
        seq_t val =
            4 * buf[(i - 2) % 16]
          - 4 * buf[(i - 4) % 16]
          - 4 * buf[(i - 6) % 16]
          + 11 * buf[(i - 8) % 16]
          - 4 * buf[(i -10) % 16]
          - 4 * buf[(i -12) % 16]
          + 4 * buf[(i -14) % 16]
          - buf[(i -16) % 16];
        buf[i % 16] = val;
    }

    return buf[n % 16];
}
