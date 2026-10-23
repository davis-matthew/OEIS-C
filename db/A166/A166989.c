/*
 * A166989  G.f.: A(x) = 1/(1 - 2*x - 7*x^2 - 2*x^3 + x^4).
 * https://oeis.org/A166989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) + 2*a(n-3) - a(n-4), a(0)=1, a(1)=2,
 *            a(2)=11, a(3)=38
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)11;
    if (n == 3) return (seq_t)38;

    seq_t old4 = (seq_t)1;   /* a(n-4) */
    seq_t old3 = (seq_t)2;   /* a(n-3) */
    seq_t old2 = (seq_t)11;  /* a(n-2) */
    seq_t old1 = (seq_t)38;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = 2 * old1 + 7 * old2 + 2 * old3 - old4;
        old4 = old3;
        old3 = old2;
        old2 = old1;
        old1 = cur;
    }
    return old1;
}
