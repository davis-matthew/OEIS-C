/*
 * A168414  a(n) = (18*n - 9*(-1)^n - 3)/4.
 * https://oeis.org/A168414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6 + 9*floor((n-1)/2) /* Derived from the explicit
 *            definition a(n) = (18*n - 9*(-1)^n - 3)/4 and the observation
 *            that the sequence alternates every two terms. The formula works
 *            for all n ≥ 1 and uses only integer arithmetic
 * Range:     u8 n <= 56, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; the caller must
       provide indices starting from 1.  For safety we return 0
       when n == 0, but this case is not part of the OEIS offset. */
    if (n == 0)
        return (seq_t)0;

    /* Compute floor((n-1)/2) using the native integer type idx_t
       (uint64_t).  The result fits in seq_t for all n where the
       final value fits in seq_t, and any overflow is unimportant
       because the value will not fit in seq_t anyway. */
    seq_t half = (seq_t)((n - 1) / 2);

    /* Final answer: 6 + 9 * floor((n-1)/2).  All constants are
       implicitly converted to seq_t by the cast operators. */
    return (seq_t)6 + (seq_t)9 * half;
}
