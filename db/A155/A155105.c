/*
 * A155105  Positive numbers appearing in the third column of A155103.
 * https://oeis.org/A155105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = a(n-1) * (1 + 3*2^(n-2)), with a(1) = 1 (conjectured
 *            product representation: a(n) = ∏_{k=1}^{n-1} (1 + 3*2^(k-1)) )
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;          /* a(1) = 1 */

    seq_t value   = (seq_t)1;     /* a(1) */
    seq_t pow2    = (seq_t)1;     /* 2^(current-2), starts at 2^0=1 for n=2 */
    seq_t three   = (seq_t)3;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t factor = (seq_t)1 + three * pow2; /* 1 + 3*2^(i-2) */
        value *= factor;                        /* a(i) = a(i-1) * factor */
        pow2   *= (seq_t)2;                     /* update 2^(i-1) for next iteration */
    }
    return value;
}
