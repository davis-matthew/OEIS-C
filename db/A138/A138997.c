/*
 * A138997  First differences of Frobenius numbers for 6 successive numbers
 *          A138986.
 * https://oeis.org/A138997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-5) - a(n-10); Initial values: a(1)=1, a(2)=1,
 *            a(3)=1, a(4)=1, a(5)=8, a(6)=2, a(7)=2, a(8)=2, a(9)=2, a(10)=14
 * Range:     u8 n <= 214, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* base values for 1 ≤ n ≤ 10 */
    static const seq_t base[11] = {0, 1, 1, 1, 1, 8,
                                   2, 2, 2, 2, 14};

    if (n <= 10)
        return base[(size_t)n];

    /* Allocate array of size n+1 (indices 0..n) */
    seq_t *buf = (seq_t *)malloc(sizeof(seq_t) * ((size_t)n + 1));
    if (!buf)
        return (seq_t)0;   /* allocation failure – unlikely for n <= 10000 */

    for (idx_t i = 1; i <= 10; ++i)
        buf[i] = base[(size_t)i];

    for (idx_t i = 11; i <= n; ++i) {
        buf[i] = buf[i - 5] * (seq_t)2 - buf[i - 10];
    }

    seq_t result = buf[n];
    free(buf);
    return result;
}
