/*
 * A053464  a(n) = n*5^(n-1).
 * https://oeis.org/A053464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*5^(n-1) (with a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 498 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow5_uint64(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 5;   /* promoted to seq_t automatically */
    while (exp)
    {
        if (exp & 1ULL)  result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow5 = pow5_uint64(n-1);   /* 5^(n-1) */
    return pow5 * (seq_t)n;          /* n * 5^(n-1) */
}
