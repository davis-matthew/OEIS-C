/*
 * A169551  Number of reduced words of length n in Coxeter group on 10
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A169551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 for n = 0; for n >= 1, a(n) = 10 * 9^(n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This counts words of length n over 10 generators with no equal neighbors.   */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)10;      /* 10 * 9^(n-1) starts with the factor 10  */
    seq_t base   = (seq_t)9;       /* base = 9                                                     */
    idx_t exp    = n - 1;          /* exponent = n-1                                               */

    while (exp)
    {
        if (exp & 1)
            result *= base;          /* multiply by current power of 9 when bit is set   */
        base *= base;                /* square base to proceed to next power of 9       */
        exp >>= 1;                   /* move to next bit of exponent                    */
    }
    return result;                   /* result now holds 10 * 9^(n-1)                     */
}
