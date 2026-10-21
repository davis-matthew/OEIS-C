/*
 * A090789  Even numbers n such that 37^2 (the square of the first irregular
 *          prime) divides the numerator of Bernoulli(n).
 * https://oeis.org/A090789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th smallest number belonging to the union of * the
 *            arithmetic progressions 284 + 1332·k (k≥0), 1184 + 1332·k (k≥0)
 *            * and 2738·k (k≥1). These are precisely the even indices n for
 *            which * 37² divides the numerator of the Bernoulli number B(n)
 * Range:     u8 none, u16 n <= 119, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>  /* for bool type, though not used in this function */

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, return 0 as a safe default */

    /* progression 1: 284 + 1332·k, k starts at 0 */
    seq_t a1 = (seq_t)284;
    /* progression 2: 1184 + 1332·k, k starts at 0 */
    seq_t a2 = (seq_t)1184;
    /* progression 3: 2738·k, k starts at 1 */
    seq_t a3 = (seq_t)2738;

    const seq_t step12 = (seq_t)1332; /* step for progressions 1 and 2 */
    const seq_t step3  = (seq_t)2738; /* step for progression 3 */

    seq_t last = 0;
    idx_t produced = 0;

    while (produced < n) {
        /* find the smallest current candidate */
        seq_t minv = a1;
        if (a2 < minv) minv = a2;
        if (a3 < minv) minv = a3;

        /* output it if it is not a duplicate of the previous term */
        if (produced == 0 || minv != last) {
            last = minv;
            produced++;
            if (produced == n) return last;
        }

        /* advance any progression that produced the current minimum */
        if (a1 == minv) a1 += step12;
        if (a2 == minv) a2 += step12;
        if (a3 == minv) a3 += step3;
    }
    /* control should never reach here */
    return last;
}
