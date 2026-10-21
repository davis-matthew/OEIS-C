/*
 * A051122  a(n) = Fibonacci(n) AND Fibonacci(n+1).
 * https://oeis.org/A051122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Fibonacci(n) & Fibonacci(n+1)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 188,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute Fibonacci(n) and Fibonacci(n+1) iteratively.
       F(0) = 0, F(1) = 1.  */
    seq_t f  = 0;   /* F(i)   */
    seq_t f1 = 1;   /* F(i+1) */

    /* Advance i from 0 to n-1, after the loop f == F(n),
       f1 == F(n+1). */
    for (idx_t i = 0; i < n; ++i) {
        seq_t tmp = f + f1;   /* F(i+2) */
        f  = f1;
        f1 = tmp;
    }

    return f & f1;  /* Bitwise AND of the two consecutive Fibonacci numbers */
}
