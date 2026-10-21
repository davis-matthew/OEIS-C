/*
 * A229016  Number of arrays of median of three adjacent elements of some
 *          length 8 0..n array, with no adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229016
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) - 35*a(n-4) + 21*a(n-5)
 *            - 7*a(n-6) + a(n-7) (n > 7) Base values: a(1)=2, a(2)=105,
 *            a(3)=830, a(4)=3527, a(5)=10860, a(6)=27379, a(7)=60180
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 54, u64 n <= 178, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base_vals[8] = {
    0,          /* unused: a(0) */
    2,          /* a(1) */
    105,        /* a(2) */
    830,        /* a(3) */
    3527,       /* a(4) */
    10860,      /* a(5) */
    27379,      /* a(6) */
    60180       /* a(7) */
};

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* defined only for n >= 1 */

    if (n <= 7) return base_vals[n];

    /* sliding window holding a(n-7) .. a(n-1) */
    seq_t w[7] = {
        base_vals[1],     /* a(1) */
        base_vals[2],     /* a(2) */
        base_vals[3],     /* a(3) */
        base_vals[4],     /* a(4) */
        base_vals[5],     /* a(5) */
        base_vals[6],     /* a(6) */
        base_vals[7]      /* a(7) */
    };

    for (idx_t i = 8; i <= n; ++i) {
        seq_t next = 7 * w[6]
                   - 21 * w[5]
                   + 35 * w[4]
                   - 35 * w[3]
                   + 21 * w[2]
                   - 7 * w[1]
                   + w[0];

        /* shift window left by one */
        w[0] = w[1];
        w[1] = w[2];
        w[2] = w[3];
        w[3] = w[4];
        w[4] = w[5];
        w[5] = w[6];
        w[6] = next;
    }

    return w[6];   /* a(n) */
}
