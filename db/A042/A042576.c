/*
 * A042576  Numerators of continued fraction convergents to sqrt(817).
 * https://oeis.org/A042576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A042576: numerators of continued fraction convergents to sqrt(817)
    Recurrence for n≥12:  a(n) = 686*a(n-6) - a(n-12)
    Initial values:
        a(0)=28, a(1)=29, a(2)=57, a(3)=143, a(4)=200, a(5)=343,
        a(6)=19408, a(7)=19751, a(8)=39159, a(9)=98069,
        a(10)=137228, a(11)=235297
*/

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base terms for indices 0..11 */
    static const seq_t base[12] = {
        (seq_t)28, (seq_t)29, (seq_t)57, (seq_t)143,
        (seq_t)200, (seq_t)343, (seq_t)19408, (seq_t)19751,
        (seq_t)39159, (seq_t)98069, (seq_t)137228, (seq_t)235297
    };

    if (n < 12)
        return base[(size_t)n];

    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = base[i];

    for (idx_t i = 12; i <= n; ++i) {
        size_t idx6  = (size_t)((i - 6) % 12);
        size_t idx12 = (size_t)((i - 12) % 12);
        buf[(size_t)(i % 12)] = ((seq_t)686) * buf[idx6] - buf[idx12];
    }
    return buf[(size_t)(n % 12)];
}
