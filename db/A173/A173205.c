/*
 * A173205  a(n) = 98*a(n-1)-a(n-2) for n>2, a(1)=1, a(2)=98.
 * https://oeis.org/A173205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=98, a(n)=98*a(n-1)-a(n-2) for n>2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)98;

    seq_t prev  = 98;   /* a(n-1) */
    seq_t prev2 = 1;    /* a(n-2) */
    seq_t curr  = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        curr = (seq_t)98 * prev - prev2;
        prev2 = prev;
        prev  = curr;
    }
    return prev;
}
