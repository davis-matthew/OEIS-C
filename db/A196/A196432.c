/*
 * A196432  Number of nX4 0..4 arrays with each element x equal to the number
 *          its horizontal and vertical neighbors equal to 2,1,0,3,4 for
 *          x=0,1,2,3,4.
 * https://oeis.org/A196432
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-2) +26*a(n-3)
 *            +57*a(n-4)+71*a(n-5)+49*a(n-6)+43*a(n-7)+35*a(n-8)+22*a(n-9)+25*a(n-10)+11*a(n-11)-5*a(n-12)-a(n-14)
 *            Initial values: a(1)=4, a(2)=20, a(3)=75, a(4)=314, a(5)=1283,
 *            a(6)=5311, a(7)=21803, a(8)=89640, a(9)=369032, a(10)=1517961,
 *            a(11)=6244222, a(12)=25688677, a(13)=105682...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[15] = {
    0,                /* dummy for index 0 */
    4,                /* a(1) */
    20,               /* a(2) */
    75,               /* a(3) */
    314,              /* a(4) */
    1283,             /* a(5) */
    5311,             /* a(6) */
    21803,            /* a(7) */
    89640,            /* a(8) */
    369032,           /* a(9) */
    1517961,          /* a(10) */
    6244222,          /* a(11) */
    25688677,         /* a(12) */
    105682189,        /* a(13) */
    434759457         /* a(14) */
};

seq_t a(idx_t n) {
    if (n <= 0) return 0;                 /* offset starts at 1 */
    if (n <= 14) return init[n];

    /* Circular buffer to hold the last 15 terms */
    seq_t buf[15] = {0};
    for (size_t i = 1; i <= 14; ++i) buf[i] = init[i];

    for (idx_t i = 15; i <= n; ++i) {
        size_t i2  = (i - 2) % 15;
        size_t i3  = (i - 3) % 15;
        size_t i4  = (i - 4) % 15;
        size_t i5  = (i - 5) % 15;
        size_t i6  = (i - 6) % 15;
        size_t i7  = (i - 7) % 15;
        size_t i8  = (i - 8) % 15;
        size_t i9  = (i - 9) % 15;
        size_t i10 = (i -10) % 15;
        size_t i11 = (i -11) % 15;
        size_t i12 = (i -12) % 15;
        size_t i14 = (i -14) % 15;

        /* Compute positive part */
        seq_t pos =
            buf[i2] * 6  + buf[i3] * 26 + buf[i4] * 57 + buf[i5] * 71 +
            buf[i6] * 49 + buf[i7] * 43 + buf[i8] * 35 + buf[i9] * 22 +
            buf[i10]* 25 + buf[i11]*11;

        /* Compute negative part */
        seq_t neg = buf[i12] * 5 + buf[i14];

        buf[i % 15] = pos - neg;
    }

    return buf[n % 15];
}
