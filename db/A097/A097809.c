/*
 * A097809  a(n) = 5*2^n - 2*n - 4.
 * https://oeis.org/A097809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*2^n - 2*n - 4
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n in seq_t.  We use a loop of multiplications by 2 to avoid
       undefined behaviour from shifting by a count >= the width of seq_t. */
    seq_t pow2 = 1;
    idx_t i;
    for (i = 0; i < n; ++i)
    {
        pow2 = pow2 + pow2;          /* multiply by 2 */
    }

    /* a(n) = 5*2^n - 2*n - 4 */
    seq_t result = pow2 * 5;
    result -= (seq_t)2 * (seq_t)n;
    result -= 4;

    return result;
}
