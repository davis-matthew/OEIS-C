/*
 * A052584  Expansion of e.g.f. (2 - 4*x + x^2)/((1 - x)*(1 - 2*x)).
 * https://oeis.org/A052584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2, a(n) = (1 + 2^(n-1)) * n! for n > 0
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    seq_t fact  = 1;   /* n! */
    seq_t power = 1;   /* 2^(n-1) */

    /* compute n! */
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* compute 2^(n-1) */
    for (idx_t i = 1; i < n; ++i) {
        power *= (seq_t)2;
    }

    /* a(n) = (1 + 2^(n-1)) * n! */
    fact *= ((seq_t)1 + power);
    return fact;
}
