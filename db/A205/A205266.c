/*
 * A205266  Number of (n+2)X3 0..1 arrays with every 3X3 subblock having three
 *          equal elements in a row horizontally, vertically or nw-to-se
 *          diagonally, and new values 0..1 introduced in row major order.
 * https://oeis.org/A205266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence a(n) = 6*a(n-1) +5*a(n-2) +26*a(n-3)
 *            -6*a(n-4) +14*a(n-5) -44*a(n-6) +23*a(n-7) +10*a(n-8) -15*a(n-9)
 *            -12*a(n-10) -12*a(n-11) with initial values: a(1)=223,
 *            a(2)=1606, a(3)=11553, a(4)=82991, a(5)=596414, a(6)=4285989,
 *            a(7)=30799890, a(8)=221334561, a(9)=1590556222, a(10)=114...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* initial 11 terms */
    static const seq_t init[11] = {
        (seq_t)223ULL,
        (seq_t)1606ULL,
        (seq_t)11553ULL,
        (seq_t)82991ULL,
        (seq_t)596414ULL,
        (seq_t)4285989ULL,
        (seq_t)30799890ULL,
        (seq_t)221334561ULL,
        (seq_t)1590556222ULL,
        (seq_t)11430067537ULL,
        (seq_t)82138844381ULL
    };

    if (n <= 11U)
        return init[n - 1U];

    seq_t last[11];
    for (size_t i = 0; i < 11; ++i)
        last[i] = init[i];

    for (idx_t cur = 12; cur <= n; ++cur)
    {
        seq_t next = 6 * last[10] +        /* a(n-1) */
                     5 * last[9]  +        /* a(n-2) */
                     26 * last[8] +        /* a(n-3) */
                    -6 * last[7]  +        /* -a(n-4) */
                     14 * last[6] +        /* a(n-5) */
                    -44 * last[5] +        /* -a(n-6) */
                     23 * last[4] +        /* a(n-7) */
                     10 * last[3] +        /* a(n-8) */
                    -15 * last[2] +        /* -a(n-9) */
                    -12 * last[1] +        /* -a(n-10) */
                    -12 * last[0];         /* -a(n-11) */

        /* shift the window */
        for (size_t i = 0; i < 10; ++i)
            last[i] = last[i + 1];
        last[10] = next;
    }

    return last[10];
}
