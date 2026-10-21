/*
 * A070967  a(n) = Sum_{k=0..n} binomial(6*n,6*k).
 * https://oeis.org/A070967
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(6*n, 6*k) Recurrence: a(0)=1,
 *            a(1)=2, a(2)=926, a(3)=37130 for n>=4: a(n)=38*a(n-1) +
 *            1691*a(n-2) - 1728*a(n-3)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t used by idx_t, may be required by build */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)926;
    if (n == 3) return (seq_t)37130;

    /* use the recurrence iteratively; constants are small,
       so multiplication is by a small integer. */
    seq_t a1 = (seq_t)2;       /* a(1) */
    seq_t a2 = (seq_t)926;     /* a(2) */
    seq_t a3 = (seq_t)37130;   /* a(3) */

    for (idx_t i = 4; i <= n; ++i) {
        seq_t t1 = a3 * (seq_t)38;
        seq_t t2 = a2 * (seq_t)1691;
        seq_t t3 = a1 * (seq_t)1728;
        seq_t an = t1 + t2 - t3;   /* a(n) = 38*a(n-1) + 1691*a(n-2) - 1728*a(n-3) */

        a1 = a2;
        a2 = a3;
        a3 = an;
    }
    return a3;
}
