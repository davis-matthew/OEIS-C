/*
 * A052934  Expansion of (1-x)/(1-6*x).
 * https://oeis.org/A052934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*6^(n-1) for n≥1; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* fast exponentiation: compute 6^(n-1) */
    idx_t exp   = n - 1;
    seq_t pow   = (seq_t)1;
    seq_t base  = (seq_t)6;

    while (exp)
    {
        if (exp & 1)
            pow *= base;
        base *= base;
        exp >>= 1;
    }

    return (seq_t)5 * pow;
}
