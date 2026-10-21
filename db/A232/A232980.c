/*
 * A232980  The Gauss factorial n_3!.
 * https://oeis.org/A232980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{1 ≤ j ≤ n, gcd(j,3)=1} j (product of all integers ≤ n
 *            that are not divisible by 3)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = 1;
    /* n may be 0; the loop then does nothing, leaving prod = 1, the correct value for a(0). */
    for(idx_t i = 1; i <= n; ++i)
    {
        /* Only multiply i when it is not a multiple of 3. */
        if ((i % 3) != 0)
        {
            prod = prod * (seq_t)i;
        }
    }
    return prod;
}
