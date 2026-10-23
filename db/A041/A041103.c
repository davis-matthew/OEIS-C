/*
 * A041103  Denominators of continued fraction convergents to sqrt(59).
 * https://oeis.org/A041103
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 1060*a(n-6) - a(n-12), for n >= 12 Initial values:
 *            a(0)=1,a(1)=1,a(2)=3,a(3)=22,a(4)=47,a(5)=69,a(6)=1013,a(7)=1082,
 *            a(8)=3177,a(9)=23321,a(10)=49819,a(11)=73140
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        (seq_t)1,  (seq_t)1,  (seq_t)3,  (seq_t)22,
        (seq_t)47, (seq_t)69, (seq_t)1013, (seq_t)1082,
        (seq_t)3177, (seq_t)23321, (seq_t)49819, (seq_t)73140
    };

    if (n < 12U)
        return init[(size_t)n];

    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    for (idx_t i = 12; i <= n; ++i) {
        seq_t next = (seq_t)1060 * buf[6] - buf[0];

        for (size_t k = 0; k < 11; ++k)
            buf[k] = buf[k + 1];
        buf[11] = next;
    }

    return buf[11];
}
