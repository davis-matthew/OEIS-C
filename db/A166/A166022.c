/*
 * A166022  a(n) = 3*a(n-1) -2*a(n-2) -a(n-3) +3*a(n-4) -2*a(n-5) for n > 4,
 *          with initial values as shown.
 * https://oeis.org/A166022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) - a(n-3) + 3*a(n-4) - 2*a(n-5) with
 *            initial values a(0)=0, a(1)=1, a(2)=2, a(3)=2, a(4)=4
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial terms */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)2;
    if (n == 4) return (seq_t)4;

    /* rolling buffer of the last five terms */
    seq_t buf[5] = {0, 1, 2, 2, 4};

    for (idx_t i = 5; i <= n; ++i) {
        /* indices modulo 5 */
        size_t i1 = (size_t)((i - 1) % 5);
        size_t i2 = (size_t)((i - 2) % 5);
        size_t i3 = (size_t)((i - 3) % 5);
        size_t i4 = (size_t)((i - 4) % 5);
        size_t i5 = (size_t)((i - 5) % 5);

        /* recurrence computation */
        seq_t val = (seq_t)3 * buf[i1]
                  - (seq_t)2 * buf[i2]
                  - buf[i3]
                  + (seq_t)3 * buf[i4]
                  - (seq_t)2 * buf[i5];

        /* store for next iterations */
        buf[i % 5] = val;
    }

    /* result is stored at position n % 5 */
    return buf[(size_t)(n % 5)];
}
