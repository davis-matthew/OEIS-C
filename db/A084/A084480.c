/*
 * A084480  Number of tilings of a 4 X 2n rectangle with L tetrominoes.
 * https://oeis.org/A084480
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + 2*a(n-2) - a(n-3) - 4*a(n-4) - 4*a(n-5) -
 *            2*a(n-6)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 30, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial values: a(0)=1, a(1)=2, a(2)=10, a(3)=42, a(4)=182, a(5)=790           */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Base cases for n ≤ 5 */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)10;
    if (n == 3) return (seq_t)42;
    if (n == 4) return (seq_t)182;
    if (n == 5) return (seq_t)790;

    /* Sliding window of the last 6 values of the sequence */
    seq_t a0 = 1,   /* a(n-6) */
          a1 = 2,   /* a(n-5) */
          a2 = 10,  /* a(n-4) */
          a3 = 42,  /* a(n-3) */
          a4 = 182, /* a(n-2) */
          a5 = 790; /* a(n-1) */

    idx_t i;
    for (i = 6; i <= n; ++i) {
        seq_t an =  (seq_t)4 * a5 + (seq_t)2 * a4
                  - a3
                  - (seq_t)4 * a2
                  - (seq_t)4 * a1
                  - (seq_t)2 * a0;

        /* Shift window: drop a(n-6) and append a(n) */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = a5;
        a5 = an;
    }

    return a5; /* a(n) is now stored in a5 */
}
