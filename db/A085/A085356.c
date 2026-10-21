/*
 * A085356  a(n) = polygorial(n,3)/polygorial(3,n), n >= 3.
 * https://oeis.org/A085356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0) = 1 ,  for n ≥ 1
 *   a(n) = a(n‑1) * (n+1) * (n+4) / 2
 */
seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* all arithmetic is performed on seq_t, using
           the expression tree a * (i+1) * (i+4) / 2,
           which is guaranteed to be integral.      */
        result = (result * (seq_t)(i + 1) * (seq_t)(i + 4)) / 2;
    }
    return result;
}
