/*
 * A099372  a(n) = A099371(n)^2.
 * https://oeis.org/A099372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 82*a(n-1) + 82*a(n-2) - a(n-3), a(0)=0, a(1)=1, a(2)=81
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 81;

    seq_t a0 = 0;   /* a(n-3)   */
    seq_t a1 = 1;   /* a(n-2)   */
    seq_t a2 = 81;  /* a(n-1)   */
    seq_t an = 0;

    for (idx_t i = 3; i <= n; ++i) {
        an = 82 * a2 + 82 * a1 - a0;
        a0 = a1;
        a1 = a2;
        a2 = an;
    }
    return a2;
}
