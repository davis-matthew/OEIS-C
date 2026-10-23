/*
 * A168963  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^22 = I.
 * https://oeis.org/A168963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 (n = 0), and a(n) = 46 * 45^(n‑1) (n ≥ 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 1 */
        return (seq_t)1;

    /* Compute 45^(n-1) by binary exponentiation */
    idx_t exp   = n - 1;
    seq_t base  = (seq_t)45;
    seq_t result = (seq_t)1;

    while (exp > 0) {
        if (exp & 1U)
            result = result * base;
        base = base * base;
        exp >>= 1U;
    }

    /* Multiply by 46 to obtain a(n) */
    result = result * (seq_t)46;
    return result;
}
