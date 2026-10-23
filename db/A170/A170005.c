/*
 * A170005  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 44 · 43^(n‑1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                     /* a(0) */
        return (seq_t)1;

    idx_t exp  = n - 1;              /* exponent for 43 */
    seq_t base = (seq_t)43;
    seq_t power = (seq_t)1;

    while (exp)                      /* binary exponentiation */
    {
        if (exp & 1U)
            power *= base;           /* multiply when bit is set */
        base *= base;
        exp >>= 1;
    }
    return power * (seq_t)44;         /* multiply by 44 */
}
