/*
 * A207584  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 62, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Empirical recurrence: a(n) = a(n-1)+a(n-2)+4*a(n-3)-2*a(n-4)-a(n-5)
 *                            -8*a(n-6)+a(n-7)-3*a(n-8)+3*a(n-9)-2*a(n-10)
 *                            -a(n-12),  for n>14
 * Initial values:
 * a(1)=6, a(2)=36, a(3)=60, a(4)=144, a(5)=312, a(6)=612,
 * a(7)=1250, a(8)=2516, a(9)=4968, a(10)=9796,
 * a(11)=19256, a(12)=37740, a(13)=73704, a(14)=143810 */

#include <stddef.h> /* for size_t */

static const seq_t init[15] = {
    0,          /* a(0) unused */
    6,          /* a(1) */
    36,         /* a(2) */
    60,         /* a(3) */
    144,        /* a(4) */
    312,        /* a(5) */
    612,        /* a(6) */
    1250,       /* a(7) */
    2516,       /* a(8) */
    4968,       /* a(9) */
    9796,       /* a(10) */
    19256,      /* a(11) */
    37740,      /* a(12) */
    73704,      /* a(13) */
    143810      /* a(14) */
};

seq_t a(idx_t n)
{
    if (n <= 14)
        return init[(size_t)n];

    /* Circular buffer holding the last 15 terms; indices 0..14 */
    seq_t buf[15];
    for (int i = 0; i < 15; ++i)
        buf[i] = init[i];

    for (idx_t i = 15; i <= n; ++i) {
        int m  = (int)(i % 15);
        int m1 = (int)((i - 1u) % 15);
        int m2 = (int)((i - 2u) % 15);
        int m3 = (int)((i - 3u) % 15);
        int m4 = (int)((i - 4u) % 15);
        int m5 = (int)((i - 5u) % 15);
        int m6 = (int)((i - 6u) % 15);
        int m7 = (int)((i - 7u) % 15);
        int m8 = (int)((i - 8u) % 15);
        int m9 = (int)((i - 9u) % 15);
        int m10 = (int)((i - 10u) % 15);
        int m12 = (int)((i - 12u) % 15);

        seq_t a1  = buf[m1];
        seq_t a2  = buf[m2];
        seq_t a3  = buf[m3];
        seq_t a4  = buf[m4];
        seq_t a5  = buf[m5];
        seq_t a6  = buf[m6];
        seq_t a7  = buf[m7];
        seq_t a8  = buf[m8];
        seq_t a9  = buf[m9];
        seq_t a10 = buf[m10];
        seq_t a12 = buf[m12];

        seq_t posSum = a1 + a2 + 4 * a3 + a7 + 3 * a9;
        seq_t negSum = 2 * a4 + a5 + 8 * a6 + 3 * a8 + 2 * a10 + a12;

        buf[m] = posSum - negSum;
    }

    return buf[(int)(n % 15)];
}
