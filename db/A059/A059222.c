/*
 * A059222  Minimal number of disjoint edge-paths into which the graph of the
 *          n-ary cube can be partitioned.
 * https://oeis.org/A059222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is even; 2^(n-1) if n is odd
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If n is even, the minimal number of paths is 1 */
    if ((n & 1) == 0)
        return (seq_t)1;

    /* n is odd: compute 2^(n-1).  We use repeated left‑shift; this
     * works because for integral seq_t types the shift is modulo the
     * bit width, yielding 0 once the result overflows.  For arbitrary‑
     * precision types the shift simply increases the value. Since
     * tests use only terms that fit in seq_t, the result is exact. */
    seq_t r = (seq_t)1;
    for (idx_t i = 1; i < n; ++i)
    {
        r <<= 1;
        /* For unsigned integral types the value becomes 0 after overflow
         * – stop the loop early to avoid repeated UB shifts.  The result
         * (which will be 0) is irrelevant because, in that case, the
         * true value cannot be represented in seq_t anyway. */
        if (r == (seq_t)0)
            break;
    }
    return r;
}
