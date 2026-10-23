/*
 * A197883  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 0,2,1,1,1
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2) + 2*a(n-3) + a(n-4) - 3*a(n-5) - 4*a(n-6) +
 *            7*a(n-7) - 5*a(n-9) with initial values a(1)=0, a(2)=4, a(3)=3,
 *            a(4)=14, a(5)=18, a(6)=43, a(7)=78, a(8)=154, a(9)=305
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 33, u64 n <= 66, u128 n <= 133,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t initial[9] = {
    0,   /* a(1) */
    4,   /* a(2) */
    3,   /* a(3) */
    14,  /* a(4) */
    18,  /* a(5) */
    43,  /* a(6) */
    78,  /* a(7) */
    154, /* a(8) */
    305  /* a(9) */
};

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined, but safe fallback */
    if (n <= 9) return initial[n - 1];

    /* circular buffer holding a(n-9) ... a(n-1) when computing a(n) */
    seq_t buf[9];
    for (size_t i = 0; i < 9; ++i)
        buf[i] = initial[i];       /* buf[0]=a1 ... buf[8]=a9 at start */

    for (idx_t cur = 10; cur <= n; ++cur) {
        /* a(cur) = 3*buf[7] + 2*buf[6] + buf[5] - 3*buf[4]
                   - 4*buf[3] + 7*buf[2] - 5*buf[0]   */
        seq_t pos = 3 * buf[7] + 2 * buf[6] + buf[5] + 7 * buf[2];
        seq_t neg = 3 * buf[4] + 4 * buf[3] + 5 * buf[0];
        seq_t new_val = pos - neg;              /* guaranteed non‑negative */

        /* Shift buffer left (drop a(cur-9)) and append a(cur) */
        for (size_t i = 0; i < 8; ++i)
            buf[i] = buf[i + 1];
        buf[8] = new_val;
    }

    return buf[8];                       /* a(n) */
}
