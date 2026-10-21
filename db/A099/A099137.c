/*
 * A099137  Iterated pentagonal numbers starting with 2.
 * https://oeis.org/A099137
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=2; a(1)=5; a(n) = (a(n-1) * (3*a(n-1)-1)) / 2 for n >= 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly */
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)5;

    seq_t prev = (seq_t)5;   /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t temp = ((seq_t)3) * prev - (seq_t)1;  /* 3*prev - 1 */
        prev = (prev * temp) / (seq_t)2;            /* pentagonal */
    }

    return prev;
}
