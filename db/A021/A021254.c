/*
 * A021254  Expansion of g.f. 1/((1-x)*(1-2*x)*(1-7*x)*(1-12*x)).
 * https://oeis.org/A021254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 22*a(n-1) -143*a(n-2) +290*a(n-3) -168*a(n-4); Initial
 *            values: a(0)=1, a(1)=22, a(2)=341, a(3)=4646
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the initial segment directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)22;
    if (n == 2) return (seq_t)341;
    if (n == 3) return (seq_t)4646;

    /* Variables hold the four most recent terms:
         a0 = a(i-4), a1 = a(i-3), a2 = a(i-2), a3 = a(i-1)
       Initially set to the known terms for i = 4 */
    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)22;
    seq_t a2 = (seq_t)341;
    seq_t a3 = (seq_t)4646;

    seq_t next = 0;
    for (idx_t i = 4; i <= n; ++i) {
        next = (seq_t)22 * a3
             - (seq_t)143 * a2
             + (seq_t)290 * a1
             - (seq_t)168 * a0;

        /* Shift the window one step forward */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return next;
}
