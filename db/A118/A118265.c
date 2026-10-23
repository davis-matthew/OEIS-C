/*
 * A118265  Coefficient of q^n in (1-q)^4/(1-4q); dimensions of the enveloping
 *          algebra of the derived free Lie algebra on 4 letters.
 * https://oeis.org/A118265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∑_{k=0}^{min(n,4)} (-1)^k * C(4,k) * 4^(n-k) * For n ≥ 4
 *            this simplifies to a(n) = 81 * 4^(n-4)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow4(idx_t e)
{
    seq_t result = 1;
    seq_t base   = 4;
    while (e)
    {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 4)
    {
        /* compute the general sum for the first few terms */
        seq_t res = 0;
        /* k = 0 */
        if (n >= 0) res += pow4(n);              /* +1*4^n  */
        /* k = 1 */
        if (n >= 1) res -= 4 * pow4(n - 1);      /* -4*4^(n-1) */
        /* k = 2 */
        if (n >= 2) res += 6 * pow4(n - 2);      /* +6*4^(n-2) */
        /* k = 3 */
        if (n >= 3) res -= 4 * pow4(n - 3);      /* -4*4^(n-3) */
        return res;                             /* C(4,4) term omitted for n<4 */
    }
    else
    {
        /* a(n) = 81 * 4^(n-4)  for n >= 4 */
        return 81 * pow4(n - 4);
    }
}
