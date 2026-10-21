/*
 * A207170  Number of 2 X n 0..1 arrays avoiding 0 0 0 and 1 0 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207170
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+3*a(n-3)+a(n-4)-a(n-5)-a(n-6) for n>7
 *            Initial values: a(1)=4, a(2)=16, a(3)=36, a(4)=81, a(5)=169,
 *            a(6)=361, a(7)=784
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 56, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, but harmless for n>=1 */

    /* handle small n directly */
    static const seq_t base[7] = {4, 16, 36, 81, 169, 361, 784};
    if (n <= 7)
        return base[n - 1];

    /* Maintain the last six values a(n-1), a(n-2), …, a(n-6) */
    seq_t last[6];
    /* For n = 8 we need a7…a2 */
    last[0] = base[6];  /* a7 */
    last[1] = base[5];  /* a6 */
    last[2] = base[4];  /* a5 */
    last[3] = base[3];  /* a4 */
    last[4] = base[2];  /* a3 */
    last[5] = base[1];  /* a2 */

    idx_t i;
    for (i = 8; i <= n; ++i) {
        /* compute a(i) */
        seq_t val = last[0] + last[1] + 3 * last[2] + last[3]
                    - last[4] - last[5];

        /* shift window: discard a(i-6), move others forward, insert new value */
        last[5] = last[4];
        last[4] = last[3];
        last[3] = last[2];
        last[2] = last[1];
        last[1] = last[0];
        last[0] = val;
    }

    return last[0]; /* a(n) */
}
