/*
 * A208553  Number of nX6 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 0 1 1 vertically.
 * https://oeis.org/A208553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +6*a(n-2) -14*a(n-3) -14*a(n-4) +42*a(n-5)
 *            +14*a(n-6) -70*a(n-7) +70*a(n-9) -14*a(n-10) -42*a(n-11)
 *            +14*a(n-12) +14*a(n-13) -6*a(n-14) -2*a(n-15) +a(n-16) Initial
 *            values: a(1) = 26, a(2) = 676, a(3) = 3354, a(4) = 16641, a(5) =
 *            50052, a(6) = 150544, a(7) = 351140, a(8) = 819025, a...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 28, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const seq_t init[16] = {
        26ULL,
        676ULL,
        3354ULL,
        16641ULL,
        50052ULL,
        150544ULL,
        351140ULL,
        819025ULL,
        1634430ULL,
        3261636ULL,
        5853246ULL,
        10504081ULL,
        17449544ULL,
        28987456ULL,
        45403272ULL,
        71115489ULL
    };

    if (n <= 16) {
        return init[n-1];
    }

    /* Buffer holds a(i-1), a(i-2), ..., a(i-16) at each iteration */
    seq_t last[16];
    last[0] = init[15]; /* a(16) */
    last[1] = init[14]; /* a(15) */
    last[2] = init[13]; /* a(14) */
    last[3] = init[12]; /* a(13) */
    last[4] = init[11]; /* a(12) */
    last[5] = init[10]; /* a(11) */
    last[6] = init[ 9]; /* a(10) */
    last[7] = init[ 8]; /* a( 9) */
    last[8] = init[ 7]; /* a( 8) */
    last[9] = init[ 6]; /* a( 7) */
    last[10] = init[ 5];/* a( 6) */
    last[11] = init[ 4];/* a( 5) */
    last[12] = init[ 3];/* a( 4) */
    last[13] = init[ 2];/* a( 3) */
    last[14] = init[ 1];/* a( 2) */
    last[15] = init[ 0];/* a( 1) */

    for (idx_t i = 17; i <= n; ++i) {
        seq_t next = 2*last[0]          /* a(n-1) */
                   + 6*last[1]          /* a(n-2) */
                   - 14*last[2]         /* a(n-3) */
                   - 14*last[3]         /* a(n-4) */
                   + 42*last[4]         /* a(n-5) */
                   + 14*last[5]         /* a(n-6) */
                   - 70*last[6]         /* a(n-7) */
                   + 70*last[8]         /* a(n-9) */
                   - 14*last[9]         /* a(n-10) */
                   - 42*last[10]        /* a(n-11) */
                   + 14*last[11]        /* a(n-12) */
                   + 14*last[12]        /* a(n-13) */
                   - 6*last[13]         /* a(n-14) */
                   - 2*last[14]         /* a(n-15) */
                   + last[15];          /* a(n-16) */

        /* Shift the buffer: last[j] becomes last[j-1] for j>=1 */
        for (int j = 15; j >= 1; --j) {
            last[j] = last[j-1];
        }
        last[0] = next;
    }

    return last[0];
}
