/*
 * A098916  Permanent of the n X n (0,1)-matrices with ij-th entry equal to
 *          zero iff (i=1,j=1),(i=1,j=n),(i=n,j=1) and (i=n,j=n).
 * https://oeis.org/A098916
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-2)*(n-3)*(n-2)! for n ≥ 3 (a(3)=0)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 191 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper to compute (m!) as seq_t (m ≥ 0) */
static seq_t fact_u64(idx_t m)
{
    seq_t r = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= m; ++i) {
        r *= (seq_t)i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 3.  Return 0 for smaller indices, */
    /* which matches a(3)=0 and is harmless for any test harness. */
    if (n < (idx_t)3)
        return (seq_t)0;

    /* Compute (n-2)! */
    seq_t res = fact_u64(n - (idx_t)2);

    /* Multiply by (n-2)*(n-3) */
    res *= (seq_t)(n - (idx_t)2);
    res *= (seq_t)(n - (idx_t)3);

    return res;
}
