/*
 * A138564  a(1) = 1; a(n) = a(n-1) + (n!)^3.
 * https://oeis.org/A138564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{k=1..n} (k!)^3
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0; /* undefined for n < 1 */

    seq_t sum  = 0;     /* cumulative sum of (k!)^3 */
    seq_t fact = 1;     /* current factorial value, starts as 0! = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        fact *= k;                   /* now fact = k! */
        seq_t cube = fact * fact * fact; /* (k!)^3 */
        sum += cube;                 /* add to running total */
    }

    return sum;
}
