/*
 * A187177  Number of 7-step left-handed knight's tours (moves only out two,
 *          left one) on an n X n board summed over all starting positions.
 * https://oeis.org/A187177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 780*n^2 - 9880*n + 29384 for n > 11 (for n ≤ 11 the small
 *            initial values are used)
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Pre‑computed values for n = 1 … 11 (index 0 unused) */
    static const seq_t table[12] = {
        0,    /* a(0) – unused */
        0,    /* a(1)  */
        0,    /* a(2)  */
        0,    /* a(3)  */
        0,    /* a(4)  */
        0,    /* a(5)  */
        16,   /* a(6)  */
        368,  /* a(7)  */
        1600, /* a(8)  */
        4284, /* a(9)  */
        8760, /* a(10) */
        15104 /* a(11) */
    };

    if (n <= 11)
        return table[(size_t)n];

    /* Empirical formula valid for n > 11 */
    seq_t N = (seq_t)n;
    seq_t val = 780 * N * N;
    val -= 9880 * N;
    val += 29384;
    return val;
}
