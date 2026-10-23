/*
 * A100304  Expansion of (1 - x - 6*x^2)/(1 - x - 8*x^2).
 * https://oeis.org/A100304
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = a(n-1) + 8*a(n-2) with a(0)=1, a(1)=0, a(2)=2
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    seq_t a0 = 1;    /* a(n-2) */
    seq_t a1 = 0;    /* a(n-1) */
    seq_t a2 = 2;    /* a(n) */

    seq_t next;
    for (idx_t i = 3; i <= n; ++i) {
        next = a1 * (seq_t)8 + a2; /* a(n) = a(n-1) + 8*a(n-2) */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
