/*
 * A028102  Expansion of 1/((1-3x)(1-8x)(1-10x)(1-11x)).
 * https://oeis.org/A028102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=32, a(2)=659, a(3)=11122,
   a(n)=32*a(n-1)-365*a(n-2)+1714*a(n-3)-2640*a(n-4) */

seq_t a(idx_t n)
{
    /* Handle the first four terms directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)32;
    if (n == 2) return (seq_t)659;
    if (n == 3) return (seq_t)11122;

    /* Iterate the recurrence up to the requested index. */
    seq_t a0 = (seq_t)1;   /* a(n-4)   */
    seq_t a1 = (seq_t)32;  /* a(n-3)   */
    seq_t a2 = (seq_t)659; /* a(n-2)   */
    seq_t a3 = (seq_t)11122; /* a(n-1)*/
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = (seq_t)32 * a3
                   - (seq_t)365 * a2
                   + (seq_t)1714 * a1
                   - (seq_t)2640 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
