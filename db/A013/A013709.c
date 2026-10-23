/*
 * A013709  a(n) = 4^(2*n+1).
 * https://oeis.org/A013709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(2*n+1) = 4*16^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A013709: a(0)=4; a(n)=16*a(n-1) for n>0 */
seq_t a(idx_t n)
{
    seq_t result = 4;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        result *= 16;          /* multiply by 16 each step */
    }
    return result;
}
