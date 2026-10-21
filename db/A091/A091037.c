/*
 * A091037  Second column (k=5) of array A090214 ((4,4)-Stirling2) divided by
 *          4*4!=96.
 * https://oeis.org/A091037
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(n-2) * (5^(n-1)-1) / 4 , n >= 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 2; return 0 for smaller
       indices to avoid UB in the tests that may call the function
       with any idx_t value. */
    if (n < 2)
        return 0;

    /* 24^(n-2)  (for n = 2 this is 24^0 = 1) */
    seq_t pow24 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow24 *= (seq_t)24;

    /* 5^(n-1)  (for n = 2 this is 5^1 = 5) */
    seq_t pow5 = (seq_t)1;
    for (idx_t i = 0; i < n - 1; ++i)
        pow5 *= (seq_t)5;

    seq_t pow5minus1 = pow5 - (seq_t)1;

    /* For n = 2 the numerator is (5^1-1) = 4, which is already
       divisible by 4; for n > 2 we can divide the 24^(n-2) factor by 4
       before the final multiplication to keep all intermediate
       results as small as possible. */
    if (n == 2)
        return pow5minus1 / (seq_t)4;

    seq_t pow24div4 = pow24 / (seq_t)4;
    return pow24div4 * pow5minus1;
}
