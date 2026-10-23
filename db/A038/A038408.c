/*
 * A038408  Coordination sequence for Zeolite Code DFT.
 * https://oeis.org/A038408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) + a(n-5) - a(n-6) - a(n-8) +
 *            a(n-9) Initial values: a(0)=1, a(1)=4, a(2)=10, a(3)=21,
 *            a(4)=36, a(5)=55, a(6)=79, a(7)=106, a(8)=138, a(9)=175
 * Range:     u8 n <= 10, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Initial 10 terms of the sequence */
    static const seq_t init[10] = {
        (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)21, (seq_t)36,
        (seq_t)55, (seq_t)79, (seq_t)106, (seq_t)138, (seq_t)175
    };

    /* Return a cached term for n < 10 */
    if (n < 10)
        return init[(size_t)n];

    /* Circular buffer of the last 10 terms */
    seq_t buf[10];
    for (size_t i = 0; i < 10; ++i)
        buf[i] = init[i];

    /* Compute successive terms up to n */
    for (idx_t i = 10; i <= n; ++i) {
        size_t i1 = (size_t)((i - 1) % 10);
        size_t i3 = (size_t)((i - 3) % 10);
        size_t i4 = (size_t)((i - 4) % 10);
        size_t i5 = (size_t)((i - 5) % 10);
        size_t i6 = (size_t)((i - 6) % 10);
        size_t i8 = (size_t)((i - 8) % 10);
        size_t i9 = (size_t)((i - 9) % 10);

        seq_t val = buf[i1] + buf[i3] - buf[i4] + buf[i5]
                    - buf[i6] - buf[i8] + buf[i9];
        buf[(size_t)(i % 10)] = val;
    }

    return buf[(size_t)(n % 10)];
}
