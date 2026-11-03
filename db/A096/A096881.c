/*
 * A096881  Expansion of g.f. (1 + 4*x)/(1 - 17*x^2).
 * https://oeis.org/A096881
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 17^floor(n/2) if n is even * a(n) = 4*17^floor(n/2) if n
 *            is odd * with a(0) = 1, a(1) = 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 25 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t pow17idx(idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)17;
    while (exp != 0)
    {
        if (exp & 1u)
            result = result * base;
        base = base * base;
        exp >>= 1u;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)4;

    idx_t exp = n >> 1;                 /* floor(n/2) */
    seq_t val = pow17idx(exp);

    if (n & 1u)            /* n is odd */
        val = val * (seq_t)4;

    return val;
}
