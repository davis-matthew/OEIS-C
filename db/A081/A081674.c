/*
 * A081674  Generalized Poly-Bernoulli numbers.
 * https://oeis.org/A081674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 19*a(n-2) + 12*a(n-3); initial values: a(0) =
 *            0, a(1) = 1, a(2) = 6
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 6;

    seq_t a0 = 0;  /* a(n-3) */
    seq_t a1 = 1;  /* a(n-2) */
    seq_t a2 = 6;  /* a(n-1) */
    seq_t val;

    for (idx_t i = 3; i <= n; ++i) {
        /* compute a(i) = 8*a2 + 12*a0 - 19*a1 */
        val = 8 * a2 + 12 * a0;   /* 8*a(n-1) + 12*a(n-3) */
        val -= 19 * a1;           /* subtract 19*a(n-2) */
        a0 = a1;
        a1 = a2;
        a2 = val;
    }
    return a2;
}
