/*
 * A076012  Sixth column of triangle A075504.
 * https://oeis.org/A076012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n * S(n+6, 6)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the n-th term of OEIS A076012, with a(0) = 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Compute 9^n */
    seq_t pow9 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow9 *= (seq_t)9;

    /* Compute S(n+6, 6) by the recurrence */
    seq_t s[7] = { (seq_t)1, 0, 0, 0, 0, 0, 0 };  /* s[0] = 1, others 0 for k = 0 */
    idx_t target = n + 6;                         /* we need S(k,6) for k = n+6 */

    for (idx_t step = 1; step <= target; ++step) {
        for (int j = 6; j >= 1; --j)
            s[j] = (seq_t)(j * s[j] + s[j - 1]); /* S(step, j) = j*S(step-1, j)+S(step-1, j-1) */
        s[0] = (seq_t)0;                        /* for step > 0, S(step,0) = 0 */
    }

    return pow9 * s[6];
}
