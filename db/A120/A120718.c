/*
 * A120718  Expansion of 3*x/(1 - 2*x^2 - 2*x + x^3).
 * https://oeis.org/A120718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2) - a(n-3), n > 2 Initial values: a(0)
 *            = 0, a(1) = 3, a(2) = 6
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)6;

    seq_t a0 = (seq_t)0;
    seq_t a1 = (seq_t)3;
    seq_t a2 = (seq_t)6;

    for (idx_t i = 3; i <= n; ++i) {
        seq_t t1 = a2 + a2;          /* 2 * a2 */
        seq_t t2 = a1 + a1;          /* 2 * a1 */
        seq_t a3 = t1 + t2 - a0;      /* a3 = 2*a2 + 2*a1 - a0 */
        a0 = a1;
        a1 = a2;
        a2 = a3;
    }

    return a2;
}
