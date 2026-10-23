/*
 * A138977  Number of 2 X n matrices containing a 1 in the top left entry, all
 *          entries are integer values and adjacent entries differ by at most
 *          1.
 * https://oeis.org/A138977
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=7*a(n-1)-4*a(n-2) with a(1)=3, a(2)=19 (OEIS A138977)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range – offset starts at 1 */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)19;

    seq_t a1 = (seq_t)3;   /* a(n-2)  */
    seq_t a2 = (seq_t)19;  /* a(n-1)  */
    seq_t nxt;
    for (idx_t i = 3; i <= n; ++i) {
        nxt = (seq_t)7 * a2 - (seq_t)4 * a1;
        a1 = a2;
        a2 = nxt;
    }
    return a2;
}
