/*
 * A041991  Denominators of continued fraction convergents to sqrt(518).
 * https://oeis.org/A041991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 17, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = 4734*a(n-6) - a(n-12) for n > 11
   a(0)=1, a(1)=1, a(2)=4, a(3)=25, a(4)=79, a(5)=104,
   a(6)=4655, a(7)=4759, a(8)=18932, a(9)=118351,
   a(10)=373985, a(11)=492336 */
#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[12] = {
        (seq_t)1, (seq_t)1, (seq_t)4,
        (seq_t)25, (seq_t)79, (seq_t)104,
        (seq_t)4655, (seq_t)4759, (seq_t)18932,
        (seq_t)118351, (seq_t)373985, (seq_t)492336
    };

    if (n < 12)
        return init[n];

    seq_t buf[12];
    for (size_t i = 0; i < 12; ++i)
        buf[i] = init[i];

    seq_t val;
    for (idx_t i = 12; i <= n; ++i) {
        unsigned idx6  = (unsigned)((i - 6) % 12);
        unsigned idx12 = (unsigned)((i - 12) % 12);
        val = (seq_t)4734 * buf[idx6] - buf[idx12];
        buf[(unsigned)(i % 12)] = val;
    }

    unsigned idx = (unsigned)(n % 12);
    return buf[idx];
}
