/*
 * A130032  Row sums of unsigned triangle A129467.
 * https://oeis.org/A130032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = \prod_{k=0}^n (k^2 - k + 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kk   = (seq_t)k * (seq_t)k;     /* k * k  */
        seq_t term = kk - (seq_t)k + 1;      /* k^2 - k + 1 */
        result *= term;                     /* product so far */
    }
    return result;
}
