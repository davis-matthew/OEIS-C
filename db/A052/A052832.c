/*
 * A052832  Expansion of e.g.f. log((-1+x)/(-1+x+x^2)).
 * https://oeis.org/A052832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A052832: Expansion of e.g.f. log((-1+x)/(-1+x+x^2)).
 *
 * Recurrence (valid for n >= 4):
 *   a(n) = (2n-2) * a(n-1) - (n-1)(n-2)(n-3) * a(n-3)
 * Initial values:
 *   a(0) = 0, a(1) = 0, a(2) = 2, a(3) = 6
 *
 * The implementation uses a circular buffer of size 4; only terms
 * a(n-1) and a(n-3) are needed for each step.
 */

#include <stdbool.h>   /* for completeness, not strictly needed */

seq_t a(idx_t n)
{
    /* base values */
    switch (n) {
    case 0: return (seq_t)0;
    case 1: return (seq_t)0;
    case 2: return (seq_t)2;
    case 3: return (seq_t)6;
    }

    /* circular buffer for the last 4 terms */
    seq_t buf[4] = { (seq_t)0, (seq_t)0, (seq_t)2, (seq_t)6 };

    for (idx_t i = 4; i <= n; ++i) {
        /* (2i-2) * a(i-1) */
        seq_t term1 = (seq_t)(2 * i - 2) * buf[(i - 1) & 3];

        /* (i-1)(i-2)(i-3) * a(i-3) */
        idx_t prod32 = (i - 1) * (i - 2) * (i - 3);
        seq_t term2 = (seq_t)prod32 * buf[(i - 3) & 3];

        buf[i & 3] = term1 - term2;
    }

    return buf[n & 3];
}
