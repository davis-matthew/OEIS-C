/*
 * A156664  Binomial transform of A052551.
 * https://oeis.org/A156664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n+3) = -2*a(n) - 3*a(n+1) + 4*a(n+2)
   a(0)=1, a(1)=2, a(2)=6 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)6;

    seq_t a0 = 1;   /* a(k-3)   */
    seq_t a1 = 2;   /* a(k-2)   */
    seq_t a2 = 6;   /* a(k-1)   */
    seq_t a_next;
    idx_t i;
    for (i = 3; i <= n; ++i)
    {
        a_next = 4 * a2 - 3 * a1 - 2 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a_next;
    }
    return a2;
}
