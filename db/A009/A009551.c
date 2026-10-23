/*
 * A009551  Expansion of sin(x)/(1-x).
 * https://oeis.org/A009551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*a(n-1) - a(n-2) + (n-2)*a(n-3); a(0)=0, a(1)=1, a(2)=2.
 *            Offset: 0
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t a0 = (seq_t)0;          /* a(n-3) */
    seq_t a1 = (seq_t)1;          /* a(n-2) */
    seq_t a2 = (seq_t)2;          /* a(n-1) */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = (seq_t)i * a2 - a1 + ((seq_t)(i - 2)) * a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return an;
}
