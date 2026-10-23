/*
 * A169396  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169396
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = 47 * 46^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t e = n - 1;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)46;

    while (e) {
        if (e & 1)
            result *= base;
        base *= base;
        e >>= 1;
    }

    return (seq_t)47 * result;
}
