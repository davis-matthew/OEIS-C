/*
 * A059145  A hierarchical sequence (S(W3{2,2}*cc) - see A059126).
 * https://oeis.org/A059145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 5*a(n-2) + 2*a(n-3) for n>2 initial values:
 *            a(0)=15, a(1)=54, a(2)=141
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 26, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)15;
    if (n == 1) return (seq_t)54;
    if (n == 2) return (seq_t)141;

    seq_t a0 = (seq_t)15;   /* a(n-3)   */
    seq_t a1 = (seq_t)54;   /* a(n-2)   */
    seq_t a2 = (seq_t)141;  /* a(n-1)   */
    seq_t next;

    /* iterate from 3 up to n */
    for (idx_t i = 3; i <= n; ++i) {
        next = 4 * a2 - 5 * a1 + 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
