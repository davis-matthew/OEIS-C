/*
 * A192742  Number of matchings in the n-antiprism graph.
 * https://oeis.org/A192742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 3*a(n-1) + 3*a(n-2) - a(n-3) - a(n-4); Initial terms:
 *            a(1) = 3, a(2) = 15, a(3) = 51, a(4) = 191
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)51;
    if (n == 4) return (seq_t)191;

    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2 = (seq_t)15;  /* a(2) */
    seq_t a3 = (seq_t)51;  /* a(3) */
    seq_t a4 = (seq_t)191; /* a(4) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 3 * a4 + 3 * a3 - a2 - a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
