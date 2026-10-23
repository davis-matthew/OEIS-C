/*
 * A037632  Base 4 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 2,1,3.
 * https://oeis.org/A037632
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 4*a(n-1) + a(n-3) - 4*a(n-4) Initial values: a(1)=2,
 *            a(2)=9, a(3)=39, a(4)=158
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but safe */
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)39;
    if (n == 4) return (seq_t)158;

    /* circular buffer holding a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t a1 = (seq_t)2;   /* a(n-4) when we are computing a_5   */
    seq_t a2 = (seq_t)9;   /* a(n-3) */
    seq_t a3 = (seq_t)39;  /* a(n-2) */
    seq_t a4 = (seq_t)158; /* a(n-1) */

    for (idx_t i = 5; i <= n; ++i) {
        seq_t next = (seq_t)4 * a4 + a2 - (seq_t)4 * a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }
    return a4;
}
