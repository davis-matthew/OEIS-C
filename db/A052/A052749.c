/*
 * A052749  a(n) = 2*n * Stirling2(n-1,2).
 * https://oeis.org/A052749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n * ( 2^(n-1) - 2 ), a(0)=0, a(1)=0, a(2)=0
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the OEIS sequence A052749. The computation works
 * for any index n, remaining efficient up to n≈10⁴. */

seq_t a(idx_t n)
{
    /* For n = 0,1,2 the sequence is zero by definition. */
    if (n < 3)
        return (seq_t)0;

    /* Compute 2^(n-1) using repeated doubling via addition.
     * This avoids undefined behaviour of shifting by more than the word size.   */
    seq_t pow2 = (seq_t)1;
    idx_t i;
    for (i = 0; i < n - 1; ++i) {
        pow2 = pow2 + pow2;   /* double the current value */
    }

    /* pow2 now holds 2^(n-1). Subtract 2 and multiply by n. */
    pow2 = pow2 - (seq_t)2;
    pow2 = pow2 * (seq_t)n;
    return pow2;
}
