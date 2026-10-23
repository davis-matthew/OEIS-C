/*
 * A168927  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 10, a(n) = 9 * a(n-1) for n >= 2 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) */
    seq_t result = (seq_t)10;     /* a(1) */
    /* For n >= 2, multiply by 9 n-1 times */
    for (idx_t i = 1; i < n; ++i)
        result *= (seq_t)9;
    return result;
}
