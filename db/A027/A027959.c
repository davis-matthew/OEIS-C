/*
 * A027959  a(n) = Sum_{k=m..n} T(k,n-k), where m = floor((n+1)/2); a(n) is
 *          the n-th diagonal-sum of left justified array T given by A027948.
 * https://oeis.org/A027959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-2)+a(n-3)-3*a(n-4)-a(n-5)+a(n-6) with a(1)=1,
 *            a(2)=1, a(3)=2, a(4)=3, a(5)=5, a(6)=7
 * Range:     u8 n <= 14, u16 n <= 28, u32 n <= 56, u64 n <= 112,
 *            u128 n <= 223, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* base values for n <= 6 */
    if (n <= 6UL) {
        static const seq_t base[6] = {
            (seq_t)1, /* a(1) */
            (seq_t)1, /* a(2) */
            (seq_t)2, /* a(3) */
            (seq_t)3, /* a(4) */
            (seq_t)5, /* a(5) */
            (seq_t)7  /* a(6) */
        };
        return base[n-1];
    }

    /* sliding window of the last six computed values */
    seq_t last[6];
    last[0] = (seq_t)1; /* a(1) */
    last[1] = (seq_t)1; /* a(2) */
    last[2] = (seq_t)2; /* a(3) */
    last[3] = (seq_t)3; /* a(4) */
    last[4] = (seq_t)5; /* a(5) */
    last[5] = (seq_t)7; /* a(6) */

    for (idx_t i = 7; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-2)+a(i-3)-3*a(i-4)-a(i-5)+a(i-6) */
        seq_t next = 3 * last[(i-3) % 6] + last[(i-4) % 6]
                    - 3 * last[(i-5) % 6] - last[(i-6) % 6]
                    + last[(i-7) % 6];
        last[(i-1) % 6] = next;
    }
    return last[(n-1) % 6];
}
