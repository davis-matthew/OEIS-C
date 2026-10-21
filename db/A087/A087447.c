/*
 * A087447  a(0) = a(1) = 1; for n > 1, a(n) = (n+2)*2^(n-2).
 * https://oeis.org/A087447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+2)*2^(n-2) for n≥2, a(0)=1, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)1;                 /* a(0)=a(1)=1 */
    /* compute 2^(n-2) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n - 2; ++i)
        pow2 = pow2 * (seq_t)2;         /* multiply by 2 each step */
    /* multiply by (n+2) */
    return (seq_t)(n + 2) * pow2;
}
