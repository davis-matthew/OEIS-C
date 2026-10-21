/*
 * A207376  Sum of central divisors of n.
 * https://oeis.org/A207376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 255, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sum of central divisors of n:
   If n is a perfect square, a(n) = sqrt(n).
   Otherwise, let d be the largest divisor ≤ sqrt(n); then
   a(n) = d + n/d.                                   */

#include <stdint.h>   /* for uint64_t, used by idx_t and seq_t in the build */

/* Integer floor square root, using only integer arithmetic. */
static inline idx_t idx_sqrt(idx_t n) {
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        /* avoid overflow: mid*mid <= n  iff  mid <= n/mid */
        if (mid == 0) {
            /* mid is 0, only possible when n==0; handle by moving low up */
            low = 1;
            continue;
        }
        if (mid <= n / mid) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return high;   /* high is the floor of sqrt(n) */
}

/* Compute the sequence value for a given n ≥ 1.   */
seq_t a(idx_t n)
{
    /* n is guaranteed ≥ 1 by the OEIS offset; guard the zero case. */
    if (n == 0) return 0;

    idx_t s = idx_sqrt(n);

    /* If n is a perfect square, the only central divisor is sqrt(n). */
    if ((seq_t)s * s == (seq_t)n)
        return (seq_t)s;

    /* Otherwise find the largest divisor ≤ sqrt(n).  */
    idx_t d = s;
    while (d > 0 && n % d != 0)
        d--;

    /* Sum the central divisor pair.  The individual terms fit in idx_t,
       but the result will be converted to the appropriate seq_t type. */
    return (seq_t)d + (seq_t)(n / d);
}
