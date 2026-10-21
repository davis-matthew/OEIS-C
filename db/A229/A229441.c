/*
 * A229441  Number of n X 4 0..2 arrays with horizontal differences mod 3
 *          never 1, vertical differences mod 3 never -1, and rows and columns
 *          lexicographically nondecreasing.
 * https://oeis.org/A229441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*a(n-1) - 36*a(n-2) + 84*a(n-3) - 126*a(n-4) * +
 *            126*a(n-5) - 84*a(n-6) + 36*a(n-7) - 9*a(n-8) + a(n-9) * where
 *            a(1)=6, a(2)=14, a(3)=37, a(4)=109, a(5)=324, a(6)=915, *
 *            a(7)=2402, a(8)=5843, a(9)=13229
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 46, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[9] = {
        (seq_t)6,   (seq_t)14,   (seq_t)37,   (seq_t)109,
        (seq_t)324, (seq_t)915,  (seq_t)2402, (seq_t)5843,
        (seq_t)13229
    };

    if (n <= 9) return init[n - 1];

    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i) buf[i] = init[i];

    for (idx_t i = 10; i <= n; ++i) {
        const seq_t next = 9 * buf[8]
                          - 36 * buf[7]
                          + 84 * buf[6]
                          - 126 * buf[5]
                          + 126 * buf[4]
                          - 84 * buf[3]
                          + 36 * buf[2]
                          - 9 * buf[1]
                          + buf[0];
        /* shift buffer left */
        for (size_t j = 0; j < 8; ++j) buf[j] = buf[j + 1];
        buf[8] = next;
    }

    return buf[8];
}
