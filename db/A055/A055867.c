/*
 * A055867  Fourth column of triangle A055864.
 * https://oeis.org/A055867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n = 1, 2, 3; a(n) = n^3 * (n+1)^(n-4) for n ≥ 4
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4) return (seq_t)0;
    seq_t N  = (seq_t)n;
    seq_t N3 = N * N * N;                     /* n^3 */
    seq_t Np1_pow = pow_seq_t(N + (seq_t)1, n - 4); /* (n+1)^(n-4) */
    return N3 * Np1_pow;
}
