/*
 * A192470  (A192469)/2.
 * https://oeis.org/A192470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=4, a(3)=22, a(4)=112 * Recurrence: a(n) = 8*a(n-1)
 *            -13*a(n-2) -12*a(n-3) +27*a(n-4) (n ≥ 5)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)22;
    if (n == 4) return (seq_t)112;

    /* a0 = a(n-4), a1 = a(n-3), a2 = a(n-2), a3 = a(n-1) */
    seq_t a0 = (seq_t)1;   /* a(1)   */
    seq_t a1 = (seq_t)4;   /* a(2)   */
    seq_t a2 = (seq_t)22;  /* a(3)   */
    seq_t a3 = (seq_t)112; /* a(4)   */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = 8 * a3 + 27 * a0 - 13 * a2 - 12 * a1;
        /* shift the window forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
