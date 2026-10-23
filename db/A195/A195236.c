/*
 * A195236  Number of lower triangles of a 6 X 6 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by one or less.
 * https://oeis.org/A195236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10654607*n - 11243757 for n>5 a(1)=2097152,
 *            a(2)=10322065, a(3)=20729610, a(4)=31374671, a(5)=42029278
 *            Recurrence for n>5: a(n)=2*a(n-1)-a(n-2)
 * Range:     u8 none, u16 none, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first five terms explicitly */
    switch (n) {
    case 1: return (seq_t)2097152U;
    case 2: return (seq_t)10322065U;
    case 3: return (seq_t)20729610U;
    case 4: return (seq_t)31374671U;
    case 5: return (seq_t)42029278U;
    default:
        /* For n >= 6 use the linear formula */
        return (seq_t)n * 10654607U - 11243757U;
    }
}
