/*
 * A129893  a(n) = s!/(s-n)! where s = (n*(n+1)/2)+1.
 * https://oeis.org/A129893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (n*(n+1)/2 + 1)! / (n*(n+1)/2 + 1 - n)!
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute s = n*(n+1)/2 + 1 using seq_t arithmetic */
    seq_t sn = (seq_t)n * ((seq_t)n + 1) / 2 + 1;

    /* falling factorial (s)_n = s * (s-1) * ... * (s-n+1) */
    seq_t result = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        result *= sn - (seq_t)i;
    }
    return result;
}
