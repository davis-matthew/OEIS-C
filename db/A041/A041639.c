/*
 * A041639  Denominators of continued fraction convergents to sqrt(338).
 * https://oeis.org/A041639
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 478*a(n-5)+a(n-10) for n>9
   Initial values: a(0)=1, a(1)=2, a(2)=3, a(3)=5, a(4)=13,
   a(5)=473, a(6)=959, a(7)=1432, a(8)=2391, a(9)=6214 */

#include <stddef.h>

static const seq_t init[10] = {
    (seq_t)1,  (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)13,
    (seq_t)473, (seq_t)959, (seq_t)1432, (seq_t)2391, (seq_t)6214
};

seq_t a(idx_t n)
{
    if (n < (idx_t)10)
        return init[(size_t)n];

    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    for (idx_t i = (idx_t)10; i <= n; ++i) {
        seq_t v5  = buf[(size_t)((i - (idx_t)5) % (idx_t)10)];
        seq_t v10 = buf[(size_t)((i - (idx_t)10) % (idx_t)10)];
        buf[(size_t)(i % (idx_t)10)] = (seq_t)478 * v5 + v10;
    }
    return buf[(size_t)(n % (idx_t)10)];
}
