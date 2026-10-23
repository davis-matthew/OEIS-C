/*
 * A169474  Number of reduced words of length n in Coxeter group on 29
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169474
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 29 * 28^(n-1) (n >= 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalently: a(0) = 1, a(1) = 29, a(n) = 28 * a(n-1) for n >= 2. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t val = (seq_t)29;
    for (idx_t i = 2; i <= n; ++i)
        val *= (seq_t)28;
    return val;
}
