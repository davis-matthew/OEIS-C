/*
 * A051490  a(n) = n^(n+2)*(n+2)^n.
 * https://oeis.org/A051490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+2)*(n+2)^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility: fast exponentiation in the same integer type as seq_t. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp > 0)
    {
        if (exp & (idx_t)1ULL)
            result = result * b;
        exp >>= 1;
        if (exp)
            b = b * b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;           /* 0^(2) * 2^0 = 0 */

    seq_t pow1 = pow_seq_t((seq_t)n, n + 2);          /* n^(n+2)   */
    seq_t pow2 = pow_seq_t((seq_t)(n + 2), n);        /* (n+2)^n   */
    return pow1 * pow2;                              /* product    */
}
