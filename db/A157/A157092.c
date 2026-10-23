/*
 * A157092  Consider all consecutive integer Pythagorean 9-tuples (X, X+1,
 *          X+2, X+3, X+4, Z-3, Z-2, Z-1, Z) ordered by increasing Z; sequence
 *          gives X values.
 * https://oeis.org/A157092
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=36, a(n)=18*a(n-1)-a(n-2)+32 for n>1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)36;

    seq_t prev2 = (seq_t)0;
    seq_t prev1 = (seq_t)36;
    seq_t curr;

    for (idx_t i = (idx_t)2; i <= n; ++i)
    {
        curr = (seq_t)18 * prev1 - prev2 + (seq_t)32;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
