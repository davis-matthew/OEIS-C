/*
 * A083589  Expansion of 1/((1-4*x)*(1-x^4)).
 * https://oeis.org/A083589
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   The sequence satisfies a(0) = 1 a(n) = 4*a(n-1) if n mod 4 ≠ 0
 *            a(n) = 4*a(n-1) + 1 if n mod 4 = 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;              /* a(0) */
    if (n == 0) return res;

    for (idx_t i = 1; i <= n; ++i) {
        res *= (seq_t)4;
        if (i % 4 == 0) res += (seq_t)1;
    }
    return res;
}
