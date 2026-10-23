/*
 * A100227  Main diagonal of triangle A100226.
 * https://oeis.org/A100227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2 for n > 1, with a(0)=1, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 50, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    seq_t a0 = (seq_t)1; /* a(0) */
    seq_t a1 = (seq_t)1; /* a(1) */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = a1 + a1;    /* 2*a(n-1) */
        a2 = a2 + a0;    /* + a(n-2) */
        a2 = a2 + (seq_t)2; /* + 2 */
        a0 = a1;
        a1 = a2;
    }
    return a1;
}
