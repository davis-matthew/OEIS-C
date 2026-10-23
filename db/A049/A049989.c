/*
 * A049989  a(n) is the number of arithmetic progressions of positive
 *          integers, nondecreasing with sum <= n.
 * https://oeis.org/A049989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of arithmetic progressions of positive integers,
 *            nondecreasing, with sum <= n. The progression is represented by
 *            its first term a (>=1), common difference d (>=0), and number of
 *            steps m (>=0). Sum of the progression: S = (m+1)*a + d*m*(m+1)/2
 * Range:     u8 n <= 24, u16 n <= 424, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle the trivial offset: a(1) is the first term */
    if (n == 0) return 0;          /* not used, undefined for n=0 */

    seq_t count = 0;

    /* m = number of steps; length L = m + 1 */
    for (idx_t m = 0; m < n; ++m) {
        if (m == 0) {
            /* length 1: d is irrelevant, only a matters */
            count += (seq_t)n;            /* a = 1 .. n  */
            continue;
        }

        /* t = m*(m+1)/2, coefficient of d in the sum */
        seq_t t = (seq_t)m * (seq_t)(m + 1) / 2;

        /* the minimal possible sum for this m is (m+1)*1.
           Any d with d*t > n-(m+1) would make even a=1 exceed n. */
        idx_t remaining = n - (m + 1);
        idx_t d_max = remaining < (idx_t)t ? 0 : remaining / (idx_t)t;

        for (idx_t d = 0; d <= d_max; ++d) {
            seq_t sub = (seq_t)d * t;                  /* d * t  */
            seq_t rem   = (seq_t)n - sub;              /* remaining sum for a */
            if (rem < (seq_t)(m + 1)) continue;        /* no a >= 1 possible */

            seq_t a_max = rem / (seq_t)(m + 1);        /* largest a allowed */
            if (a_max >= (seq_t)1)
                count += a_max;
        }
    }

    return count;
}
