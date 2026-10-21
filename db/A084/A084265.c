/*
 * A084265  a(n) = (n^2 + 3*n + 1 + (-1)^n) / 2.
 * https://oeis.org/A084265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^2 + 3*n + 1 + (-1)^n) / 2
 * Range:     u8 n <= 14, u16 n <= 263, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;                      /* convert index to seq_t          */
    seq_t total = sn * sn + (seq_t)3 * sn + (seq_t)1; /* n^2 + 3n + 1            */
    if ((n & 1) == 0)                          /* n even -> add 1               */
        total += (seq_t)1;
    else                                       /* n odd  -> subtract 1          */
        total -= (seq_t)1;
    return total / 2;                           /* divide by 2, integer division */
}
