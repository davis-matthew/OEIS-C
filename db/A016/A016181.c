/*
 * A016181  Expansion of g.f. 1/((1-7*x)*(1-10*x)).
 * https://oeis.org/A016181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n >= 1, a(n) = 10*a(n-1) + 7^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;          /* a(0) */
    seq_t pgpow7 = 1;       /* 7^0 */

    if (n == 0)
        return res;

    for (idx_t i = 1; i <= n; ++i) {
        pgpow7 = pgpow7 * (seq_t)7;          /* compute 7^i */
        res = res * (seq_t)10 + pgpow7;      /* compute a(i) */
    }

    return res;
}
