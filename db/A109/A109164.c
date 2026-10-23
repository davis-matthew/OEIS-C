/*
 * A109164  a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3), n >= 3; a(0)=1, a(1)=6,
 *          a(2)=20.
 * https://oeis.org/A109164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 4*a(n-2) + a(n-3); a(0)=1, a(1)=6, a(2)=20
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 21, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)20;

    seq_t a0 = (seq_t)1;   /* a(n-3) */
    seq_t a1 = (seq_t)6;   /* a(n-2) */
    seq_t a2 = (seq_t)20;  /* a(n-1) */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)4 * a2 - (seq_t)4 * a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
