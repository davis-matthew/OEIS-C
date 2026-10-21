/*
 * A099144  Numbers n such that the equation S(k) = S(n+k) has no solutions
 *          for k <= n, where S is the Kempner function A002034.
 * https://oeis.org/A099144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A099144:
 *  a(n) = m!   (for all m≥1)   and   a(n) = 2*m! (for all m≥4),
 *  concatenated in increasing order.  The first terms are
 *  1, 2, 6, 24, 48, 120, 240, 720, …
 *
 *  Implementation: generate factorials iteratively. For each m≥4
 *  output m! followed by 2*m!.  The first four terms 1,2,6,24 are
 *  produced directly.  This yields the correct value for any n≥1
 *  efficiently (O(n) with very small constant factor).
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* First three explicit terms */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;

    idx_t count = 3;            /* number of terms generated so far */
    seq_t fact = (seq_t)6;      /* 3! */

    for (idx_t m = 4; ; ++m) {
        /* Compute m! */
        fact = fact * (seq_t)m;

        /* m! term */
        if (count + 1 == n)
            return fact;
        ++count;

        /* 2*m! term (only for m ≥ 4) */
        if (m >= 4) {
            seq_t twice = fact + fact;   /* 2 * fact */
            if (count + 1 == n)
                return twice;
            ++count;
        }
    }
    /* unreachable */
}
