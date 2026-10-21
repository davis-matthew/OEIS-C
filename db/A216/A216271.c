/*
 * A216271  Expansion of (1-x)/((1-2x)(1-4x+x^2)).
 * https://oeis.org/A216271
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 2*a(n-3) with initial values
 *            a(0)=1, a(1)=5, a(2)=21
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)21;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)5;   /* a(n-2) */
    seq_t a2 = (seq_t)21;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = 6 * a2 + 2 * a0 - 9 * a1;
        a0 = a1;
        a1 = a2;
        a2 = cur;
    }
    return cur;
}
