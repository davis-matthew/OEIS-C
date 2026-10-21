/*
 * A211331  Number of (n+1) X (n+1) -5..5 symmetric matrices with every 2 X 2
 *          subblock having sum zero and one or three distinct values.
 * https://oeis.org/A211331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 4*a(n-1) + 4*a(n-2) - 32*a(n-3) + 8*a(n-4)
 *               + 95*a(n-5) - 60*a(n-6) - 130*a(n-7) + 101*a(n-8)
 *               + 81*a(n-9) - 64*a(n-10) - 18*a(n-11) + 12*a(n-12)
 * Initial values (n = 1 … 12):
 * 47, 99, 201, 397, 789, 1531, 2999, 5801, 11307, 21927, 42763, 83311 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    /* Cached buffer: a(1) … a(cur)   (cur is the highest computed index) */
    static seq_t *buf = NULL;
    static idx_t cur  = 0;

    /* Grow buffer if necessary */
    if (n > cur) {
        seq_t *tmp = (seq_t *)realloc(buf, n * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* unlikely in the tests */
        buf = tmp;

        /* Initial known values (fixed for n <= 12) */
        static const seq_t init[12] = {
            47, 99, 201, 397, 789, 1531,
            2999, 5801, 11307, 21927, 42763, 83311
        };

        for (idx_t i = cur + 1; i <= n; ++i) {
            if (i <= 12) {
                buf[i - 1] = init[i - 1];
            } else {
                /* Compute using only +, -, * to keep arbitrary‑precision
                 * support functional.  Accumulate positives and negatives
                 * separately to avoid intermediate underflow for unsigned
                 * types. */
                seq_t pos = 0;   /* sum of positive contributions */
                seq_t neg = 0;   /* sum of negative contributions */

                pos += (seq_t)4  * buf[i - 2];
                pos += (seq_t)4  * buf[i - 3];
                neg += (seq_t)32 * buf[i - 4];
                pos += (seq_t)8  * buf[i - 5];
                pos += (seq_t)95 * buf[i - 6];
                neg += (seq_t)60 * buf[i - 7];
                neg += (seq_t)130 * buf[i - 8];
                pos += (seq_t)101 * buf[i - 9];
                pos += (seq_t)81  * buf[i -10];
                neg += (seq_t)64  * buf[i -11];
                neg += (seq_t)18  * buf[i -12];
                pos += (seq_t)12  * buf[i -13];

                buf[i - 1] = pos - neg;   /* final result */
            }
        }
        cur = n;
    }

    return buf[n - 1];
}
