/*
 * A192760  Coefficient of x in the reduction by x^2->x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192760
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*A000045(n+3)-n-4 (where A000045 is the Fibonacci
 *            sequence)
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute F(n+3) iteratively using the Fibonacci recurrence */
    seq_t f0 = 0;        /* F(0) */
    seq_t f1 = 1;        /* F(1) */
    for (idx_t i = 0; i < n + 3; ++i)
    {
        seq_t f2 = f0 + f1; /* F(k+2) = F(k) + F(k+1) */
        f0 = f1;
        f1 = f2;
    }
    /* After the loop, f0 == F(n+3) */
    seq_t result = f0 + f0; /* 2 * F(n+3) */
    result -= (seq_t)n;
    result -= 4;
    return result;
}
