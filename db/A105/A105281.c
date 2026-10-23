/*
 * A105281  a(0)=0; a(n) = 6*a(n-1) + 6.
 * https://oeis.org/A105281
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; a(n) = 6 * a(n-1) + 6 (for n ≥ 1)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent closed form: a(n) = 6 * (6^n – 1) / 5 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t res = 0;
    for (idx_t i = 1; i <= n; ++i)
        res = res * 6 + 6;
    return res;
}
