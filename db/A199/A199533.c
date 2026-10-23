/*
 * A199533  Number of -n..n arrays x(0..5) of 6 elements with zero sum and no
 *          two consecutive zero elements.
 * https://oeis.org/A199533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3) - 15*a(n-4) + 6*a(n-5) -
 *            a(n-6) (valid for n > 6). Initial values: a(1)=80, a(2)=1414,
 *            a(3)=8342, a(4)=30484, a(5)=84852, a(6)=197962
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 47, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;

    static const seq_t init_vals[7] = {0,
        (seq_t)80,
        (seq_t)1414,
        (seq_t)8342,
        (seq_t)30484,
        (seq_t)84852,
        (seq_t)197962};

    if (n <= 6) return init_vals[(size_t)n];

    /* last six values: v[0] = a(n-6), ..., v[5] = a(n-1) */
    seq_t v[6];
    v[0] = init_vals[1];
    v[1] = init_vals[2];
    v[2] = init_vals[3];
    v[3] = init_vals[4];
    v[4] = init_vals[5];
    v[5] = init_vals[6];

    for (idx_t i = 7; i <= n; ++i) {
        seq_t next = 6 * v[5] - 15 * v[4] + 20 * v[3] - 15 * v[2] + 6 * v[1] - v[0];
        /* shift the window: discard oldest value, shift others left, insert next */
        v[0] = v[1];
        v[1] = v[2];
        v[2] = v[3];
        v[3] = v[4];
        v[4] = v[5];
        v[5] = next;
    }

    return v[5];
}
