/*
 * A103947  a(n) is the number of distinct n-th powers of functions {1, 2} ->
 *          {1, 2}.
 * https://oeis.org/A103947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1; for n≥1, a(n)=4 if n is odd, a(n)=3 if n is even
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: for n>2, a(n)=a(n-2).  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    return (seq_t)((n & 1) ? 4 : 3);
}
