/*
 * A168329  a(n) = (3/2)*(2*n - (-1)^n - 1).
 * https://oeis.org/A168329
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = (3/2)*(2*n - (-1)^n - 1)
 * Range:     u8 n <= 86, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n >= 1, a(n) = 3*n if n is odd,
       and a(n) = 3*(n-1) if n is even. */
    if (n == 0)  /* offset is 1; behaviour below does not matter */
        return 0;

    seq_t k = (seq_t)n;
    if ((n & 1ULL) == 0)  /* even n: subtract 1 before multiplying by 3 */
        k -= 1;
    return 3 * k;
}
