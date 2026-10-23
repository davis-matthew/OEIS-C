/*
 * A036837  Schoenheim bound L_1(n,n-5,n-6).
 * https://oeis.org/A036837
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 35, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) satisfies the linear recurrence
 *   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + 2*a(n-5)
 *          - 5*a(n-6) + 10*a(n-7) - 10*a(n-8) + 5*a(n-9) - a(n-10)
 * for all n ≥ 17.
 * Initial values (offset 7): a(7)=4, a(8)=11, a(9)=25, a(10)=50,
 * a(11)=92, a(12)=158, a(13)=257, a(14)=400, a(15)=600, a(16)=873.
 */

#include <stddef.h>    /* for size_t */

seq_t a(idx_t n) {
    /* Handle indices below the offset */
    if (n < 7ULL)
        return 0;

    /* Return initial terms directly */
    if (n <= 16ULL) {
        static const seq_t init[10] = {
            4, 11, 25, 50, 92, 158, 257, 400, 600, 873
        };
        return init[(size_t)(n - 7ULL)];
    }

    /* Compute terms using the recurrence iteratively. */
    seq_t buf[10];
    /* Initialise buffer with a(7) ... a(16) */
    buf[0] = 4;   buf[1] = 11;   buf[2] = 25;
    buf[3] = 50;  buf[4] = 92;   buf[5] = 158;
    buf[6] = 257; buf[7] = 400;  buf[8] = 600;
    buf[9] = 873;

    /* For each n ≥ 17 compute the next term */
    for (idx_t i = 17; i <= n; ++i) {
        __int128 v = 5    * (__int128)buf[9]   /* a(n-1) */
                   -10   * (__int128)buf[8]   /* a(n-2) */
                   +10   * (__int128)buf[7]   /* a(n-3) */
                   -5    * (__int128)buf[6]   /* a(n-4) */
                   +2    * (__int128)buf[5]   /* a(n-5) */
                   -5    * (__int128)buf[4]   /* a(n-6) */
                   +10   * (__int128)buf[3]   /* a(n-7) */
                   -10   * (__int128)buf[2]   /* a(n-8) */
                   +5    * (__int128)buf[1]   /* a(n-9) */
                   -1    * (__int128)buf[0];  /* a(n-10) */

        /* Shift buffer left by one to make room */
        for (int j = 0; j < 9; ++j)
            buf[j] = buf[j + 1];
        buf[9] = (seq_t)v;
    }

    return buf[9];
}
