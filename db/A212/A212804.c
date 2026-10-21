/*
 * A212804  Expansion of (1 - x)/(1 - x - x^2).
 * https://oeis.org/A212804
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 1, a(1) = 0
 * Range:     u8 n <= 14, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 187,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)0; /* a(1) */
    seq_t ai;
    for (idx_t i = 2; i <= n; ++i) {
        ai = a0 + a1;
        a0 = a1;
        a1 = ai;
    }
    return a1;
}
