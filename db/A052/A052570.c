/*
 * A052570  E.g.f.: x/(1-4*x).
 * https://oeis.org/A052570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n == 0; otherwise a(n) = 4^(n-1) * n! (n >= 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0U) {
        return (seq_t)0;
    }

    seq_t fact = 1;   /* n!      */
    seq_t pow4 = 1;   /* 4^(n-1) */

    /* iterate from 2 to n to compute factorial and 4^(n-1) */
    for (idx_t i = 2U; i <= n; ++i) {
        fact *= (seq_t)i;
        pow4 *= (seq_t)4;
    }

    return fact * pow4;
}
