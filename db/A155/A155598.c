/*
 * A155598  a(n) = 7^n-2^n+1.
 * https://oeis.org/A155598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n - 2^n + 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast exponentiation for the base 7 */
    seq_t res7 = 1;
    seq_t base7 = (seq_t)7;
    idx_t e = n;
    while (e)
    {
        if (e & 1U)
            res7 *= base7;
        base7 *= base7;
        e >>= 1;
    }

    /* fast exponentiation for the base 2 */
    seq_t res2 = 1;
    seq_t base2 = (seq_t)2;
    e = n;
    while (e)
    {
        if (e & 1U)
            res2 *= base2;
        base2 *= base2;
        e >>= 1;
    }

    return res7 - res2 + (seq_t)1;
}
