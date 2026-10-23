/*
 * A169460  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169460
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = 15 * 14^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t exp = n - 1;
    seq_t base = (seq_t)14;
    seq_t result = (seq_t)1;

    while (exp > 0)
    {
        if (exp & 1ULL)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }

    return result * (seq_t)15;
}
