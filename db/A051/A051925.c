/*
 * A051925  a(n) = n*(2*n+5)*(n-1)/6.
 * https://oeis.org/A051925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(2*n+5)*(n-1)/6
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th term of OEIS sequence A051925.  The function
 * works for all n ≥ 0 and for any integer type used for seq_t and
 * idx_t.  It reduces the fraction by cancelling factors 2 and 3
 * before the final multiplication to avoid overflow on fixed-width
 * integer builds. */
seq_t a(idx_t n)
{
    /* For n < 2 the formula yields zero, matching the first two
     * terms of the sequence (a(0)=0, a(1)=0).  Bypass the generic
     * computation to avoid an underflow when (n-1) would be
     * negative in an unsigned type. */
    if (n < 2)
        return (seq_t)0;

    seq_t a_val = (seq_t)n;          /* n                       */
    seq_t b_val = a_val - 1;         /* n-1                     */
    seq_t c_val = a_val * (seq_t)2 + (seq_t)5;  /* 2n+5                 */

    /* Cancel a factor of 2   */
    if ((a_val & (seq_t)1) == 0)      /* a_val even          */
        a_val >>= 1;
    else if ((b_val & (seq_t)1) == 0) /* b_val even          */
        b_val >>= 1;
    else
        c_val >>= 1;                 /* c_val must be even   */

    /* Cancel a factor of 3   */
    if (a_val % (seq_t)3 == 0)
        a_val /= (seq_t)3;
    else if (b_val % (seq_t)3 == 0)
        b_val /= (seq_t)3;
    else
        c_val /= (seq_t)3;           /* one of a_val, b_val,
                                      * c_val is always divisible
                                      * by 3, so this is safe.   */

    /* Final product gives the desired result.  No overflow occurs on
     * the commonly used 64‑bit builds because the intermediate
     * products are bounded by the final value. */
    return a_val * b_val * c_val;
}
