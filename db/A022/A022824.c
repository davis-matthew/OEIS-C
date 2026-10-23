/*
 * A022824  a(n) = [ (2n+2)/(n-1) ] + [ (2n+4)/(n-2) ] + ... + [ (4n-2)/1 ].
 * https://oeis.org/A022824
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=1}^{n-1} floor( (2n+2k)/(n-k) )
 * Range:     u8 n <= 20, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;             /* undefined for n < 2, value does not matter */

    seq_t sum = 0;
    for (idx_t k = 1; k < n; ++k)    /* k = 1 … n‑1 */
    {
        seq_t num = (seq_t)n * 2 + (seq_t)k * 2;  /* 2n + 2k */
        seq_t den = n - k;                        /* n – k   */
        sum += num / den;                         /* floor division */
    }
    return sum;
}
