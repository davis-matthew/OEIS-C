/*
 * A169564  Number of reduced words of length n in Coxeter group on 23
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A169564:
   a(0) = 1
   a(1) = 23
   a(n) = 22 * a(n-1)  for n ≥ 2
   Equivalently: a(n) = 23 * 22^(n-1) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)23;

    seq_t result = (seq_t)23;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)     /* multiply by 22 for each step */
        result *= (seq_t)22;
    return result;
}
