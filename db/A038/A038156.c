/*
 * A038156  a(n) = n! * Sum_{k=1..n-1} 1/k!.
 * https://oeis.org/A038156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=0; for n>1: a(n) = n * (a(n-1) + 1)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = n * (a(n-1) + 1) for n >= 2 */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;
    seq_t prev = 0; /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t t = prev + 1;
        prev = (seq_t)i * t;
    }
    return prev;
}
