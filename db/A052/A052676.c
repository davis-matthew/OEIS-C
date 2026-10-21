/*
 * A052676  Expansion of e.g.f. 3*x/(1 - 2*x).
 * https://oeis.org/A052676
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*2^(n-1)*n! for n > 0 * Recurrence: a(0) = 0, a(1) = 3,
 *            a(n) = 2 * n * a(n-1) (n >= 2) * This implementation evaluates
 *            the recurrence iteratively. * It works for any supported seq_t
 *            type (fixed or arbitrary precision)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t result = (seq_t)3;            /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t mult = (seq_t)2 * (seq_t)i; /* 2*i */
        result = result * mult;          /* a(i) = 2*i*a(i-1) */
    }
    return result;
}
