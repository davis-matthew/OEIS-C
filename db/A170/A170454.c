/*
 * A170454  Number of reduced words of length n in Coxeter group on 13
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^45 = I.
 * https://oeis.org/A170454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0) else 13 * 12^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the Coxeter group on 13 generators with m_ij = 45. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Compute 12^(n-1) by exponentiation by squaring. */
    seq_t base   = (seq_t)12;
    seq_t power  = (seq_t)1;
    idx_t   exp   = n - 1;

    while (exp) {
        if (exp & 1)
            power = power * base;
        base = base * base;
        exp >>= 1;
    }

    return (seq_t)13 * power;
}
