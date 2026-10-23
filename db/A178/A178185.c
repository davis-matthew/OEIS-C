/*
 * A178185  Numerator of Sum_{k=1..n} 1/2^((k^2 + 3*k)/2).
 * https://oeis.org/A178185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^(n+1) * a(n-1) + 1 a(1) = 1 (offset 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* We always start from a(1) = 1 */
    seq_t val = 1;

    /* If n == 1 just return the starting value */
    if (n == 1)
        return val;

    /* Compute iteratively up to the required index.  The recurrence
       multiplies the previous term by 2^(i+1) and adds 1.  The shift
       operator is used to effect the multiplication by 2^(i+1). */
    for (idx_t i = 2; i <= n; ++i)
    {
        val <<= (i + 1);   /* val = val * 2^(i+1) */
        val += 1;          /* val = val + 1      */
    }

    return val;
}
