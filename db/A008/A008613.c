/*
 * A008613  Molien series for 3-dimensional representation of A_5.
 * https://oeis.org/A008613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = -a(n-1)+a(n-2)+2*a(n-3)+a(n-4)
 *            -a(n-7)-2*a(n-8)-a(n-9)+a(n-10)+a(n-11), with initial values:
 *            a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=0, a(6)=2, a(7)=0,
 *            a(8)=2, a(9)=0, a(10)=3
 * Range:     u8 n <= 167, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Initial terms for 0 ≤ n ≤ 10 */
    static const seq_t init[11] = {
        (seq_t)1, (seq_t)0, (seq_t)1, (seq_t)0, (seq_t)1,
        (seq_t)0, (seq_t)2, (seq_t)0, (seq_t)2, (seq_t)0, (seq_t)3
    };

    /* For indices below the offset, use the pre‑computed constants. */
    if (n < 11)
        return init[(int)n];

    /* Sliding window holding a(k-11),...,a(k-1) for the current k. */
    seq_t v[11];
    for (int i = 0; i < 11; ++i)
        v[i] = init[i];

    /* Compute successive terms until n. */
    for (idx_t k = 11; k <= n; ++k) {
        long long val = -(long long)v[10] + (long long)v[9] + 2LL * (long long)v[8]
                        + (long long)v[7] - (long long)v[4]
                        - 2LL * (long long)v[3] - (long long)v[2]
                        + (long long)v[1] + (long long)v[0];

        /* Shift the window left by one position. */
        for (int i = 0; i < 10; ++i)
            v[i] = v[i + 1];
        v[10] = (seq_t)val;
    }
    return v[10];
}
