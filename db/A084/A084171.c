/*
 * A084171  Binomial transform of generalized Jacobsthal numbers A084170.
 * https://oeis.org/A084171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)=5·3^(n-1) – 2^n for n ≥ 1 Recurrence: a(0)=1,
 *            a(1)=3, a(n)=5·a(n-1) – 6·a(n-2) (Both definitions are
 *            equivalent.)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0)
        return (seq_t)1;

    /* compute 3^(n-1) and 2^n by repeated multiplication.
       This keeps all intermediate results ≤ the desired value
       so that it works even when seq_t is a small fixed-width type. */
    seq_t pow3 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow3 *= (seq_t)3;

    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    /* a(n) = 5 * 3^(n-1) – 2^n */
    return (seq_t)5 * pow3 - pow2;
}
