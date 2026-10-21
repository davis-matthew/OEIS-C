/*
 * A232617  Product of first n odd numbers plus product of first n even
 *          numbers: (2n-1)!! + (2n)!!, where k!! = A006882(k).
 * https://oeis.org/A232617
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n-1)!! + (2n)!!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = (1 * 3 * 5 * ... * (2n-1)) + (2 * 4 * 6 * ... * 2n) */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of offset range, not used */

    seq_t odd  = (seq_t)1;
    seq_t even = (seq_t)1;

    for (idx_t i = 1; i <= n; ++i)
    {
        odd  *= (seq_t)(2 * i - 1);
        even *= (seq_t)(2 * i);
    }

    return odd + even;
}
