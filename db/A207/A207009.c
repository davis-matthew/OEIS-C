/*
 * A207009  Number of n X 2 0..2 arrays avoiding the patterns z z+1 z or z z-1
 *          z in any row or column.
 * https://oeis.org/A207009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 6*a(n-2) + 22*a(n-3) + 114*a(n-4) + 104*a(n-5)
 *            - 184*a(n-6) - 352*a(n-7) - 128*a(n-8) - 64*a(n-9) + 32*a(n-10)
 *            Initial values: a(1)=9, a(2)=81, a(3)=529, a(4)=3721,
 *            a(5)=25921, a(6)=180625, a(7)=1256641, a(8)=8743849,
 *            a(9)=60855601, a(10)=423577561. Note: a(n) equals the square...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t init[10] = {
    (seq_t)9,
    (seq_t)81,
    (seq_t)529,
    (seq_t)3721,
    (seq_t)25921,
    (seq_t)180625,
    (seq_t)1256641,
    (seq_t)8743849,
    (seq_t)60855601,
    (seq_t)423577561
};

seq_t a(idx_t n) {
    if (n <= 10) {
        return init[n - 1];
    }

    seq_t buf[10];
    for (idx_t i = 0; i < 10; ++i) {
        buf[i] = init[i];
    }

    for (idx_t k = 10; k < n; ++k) {
        /* Compute a(k+1) from a(k) ... a(k-9) */
        seq_t next = 7 * buf[9]
                   - 6 * buf[8]
                   + 22 * buf[7]
                   + 114 * buf[6]
                   + 104 * buf[5]
                   - 184 * buf[4]
                   - 352 * buf[3]
                   - 128 * buf[2]
                   - 64  * buf[1]
                   + 32  * buf[0];

        /* Shift buffer left by one position */
        for (int i = 0; i < 9; ++i) {
            buf[i] = buf[i + 1];
        }
        buf[9] = next;
    }

    return buf[9];
}
