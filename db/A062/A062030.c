/*
 * A062030  Group even numbers into (2,4), (6,8,10,12), (14,16,18,20,22,24),
 *          ...; a(n) = product of n-th group.
 * https://oeis.org/A062030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{2n} * ( m0 * (m0+1) * ... * (m0+2n-1) ) where m0 = 1 +
 *            (n-1)*n (group n has 2n even numbers starting at 2*m0) The
 *            product is evaluated in seq_t; if seq_t is an arbitrary‑
 *            precision type this works for all n >= 1. The routine does not
 *            depend on any external state and runs in O(n) time
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* 0 is never used, but safe */

    /* compute first multiplier index m0 = 1 + (n-1)*n  */
    seq_t m0 = (seq_t)1 + ((seq_t)(n-1)) * (seq_t)n;

    /* product of the 2n consecutive integers m0 .. m0+2n-1  */
    seq_t prod = (seq_t)1;
    idx_t len = 2 * n;                     /* 2n < 2*UINT64_MAX, safe */
    for (idx_t i = 0; i < len; ++i) {
        prod *= (m0 + (seq_t)i);
    }

    /* multiply by 2^{2n} */
    for (idx_t i = 0; i < len; ++i) {
        prod *= (seq_t)2;
    }

    return prod;
}
