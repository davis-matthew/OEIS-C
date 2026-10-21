/*
 * A087290  Number of pairs of polynomials (f,g) in GF(3)[x] satisfying deg(f)
 *          <= n, deg(g) <= n and gcd(f,g) = 1.
 * https://oeis.org/A087290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*3^(2*n+1) + 2
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow3(idx_t exp)
{
    seq_t result = 1;
    seq_t base   = 3;
    while (exp)
    {
        if (exp & 1U)
            result *= base;
        base *= base;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* exponent = 2*n + 1 */
    idx_t exponent = n + n + 1U;   /* avoid overflow for small n   */
    seq_t p = pow3(exponent);
    return p * 2 + 2;
}
