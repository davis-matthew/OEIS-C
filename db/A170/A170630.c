/*
 * A170630  Number of reduced words of length n in Coxeter group on 45
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = 45 * 44^(n-1) for n ≥ 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent recurrence: a(0)=1, a(1)=45, a(n)=44*a(n-1) for n≥2 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    seq_t res = (seq_t)45;
    for (idx_t i = 1; i < n; ++i) {
        res *= (seq_t)44;
    }
    return res;
}
