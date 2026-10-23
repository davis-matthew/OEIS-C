/*
 * A169462  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^33 = I.
 * https://oeis.org/A169462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0), 17 * 16^(n-1) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    idx_t e = n - 1;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)16;

    while (e > (idx_t)0)
    {
        if (e & (idx_t)1)
            result = result * base;
        base   = base * base;
        e     >>= 1;
    }

    return result * (seq_t)17;
}
