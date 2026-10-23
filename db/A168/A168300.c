/*
 * A168300  a(n) = 6*n - a(n-1) - 2 with a(1)=5.
 * https://oeis.org/A168300
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n - 3*(-1)^n + 1)/2 Which simplifies to: a(n) = 3*n +
 *            2 if n is odd a(n) = 3*n - 1 if n is even (a(1)=5, a(2)=5, …)
 * Range:     u8 n <= 84, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle n < 1 gracefully – only required that a(1) is correct */
    if (n == 0)
        return 0;

    /* Compute using only integer arithmetic as required. */
    if (n & 1) /* odd n */
        return ((seq_t)n * 3) + 2;
    else       /* even n */
        return ((seq_t)n * 3) - 1;
}
