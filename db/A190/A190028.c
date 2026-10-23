/*
 * A190028  Number of (n+2) X 6 binary arrays with every 3 X 3 subblock
 *          commuting with each horizontal and vertical neighbor 3 X 3
 *          subblock.
 * https://oeis.org/A190028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 21, u32 n <= 58, u64 n <= 130, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A190028 is defined by the 7‑term recurrence
 *   a(n) = 2*a(n-1) - a(n-2) + a(n-3) - 2*a(n-4) + a(n-5)
 *          + a(n-6) - a(n-7)   for n > 12
 * with initial values
 *   a(1)=415, a(2)=155, a(3)=274, a(4)=458, a(5)=578,
 *   a(6)=670, a(7)=790, a(8)=966, a(9)=1242,
 *   a(10)=1702, a(11)=2338, a(12)=3247.
 */

seq_t a(idx_t n)
{
    /* Initial 12 terms, 0‑based index in array */
    static const seq_t init[12] = {
        415, 155, 274, 458, 578,
        670, 790, 966, 1242, 1702,
        2338, 3247
    };

    if (n == 0) {
        return init[0];            /* undefined for n=0, but return a(1) */
    }

    if (n <= 12) {
        return init[n - 1];
    }

    /* For n > 12: use the recurrence, keeping the last 7 values. */
    seq_t last[7];
    int i;
    /* last[0] = a(n-7), ..., last[6] = a(n-1) for n = 13 initially */
    for (i = 0; i < 7; ++i) {
        last[i] = init[5 + i];    /* init[6] .. init[11] */
    }

    seq_t cur = 0;                /* placeholder */

    for (idx_t k = 13; k <= n; ++k) {
        cur = 2 * last[6] - last[5] + last[4]
              - 2 * last[3] + last[2] + last[1] - last[0];

        /* shift left: drop last[0], move others up, and set last[6] */
        for (i = 0; i < 6; ++i) {
            last[i] = last[i + 1];
        }
        last[6] = cur;
    }

    return cur;   /* cur and last[6] hold a(n) */
}
