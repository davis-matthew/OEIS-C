/*
 * A077824  Expansion of (1-x)^(-1)/(1-3*x-2*x^2-2*x^3).
 * https://oeis.org/A077824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=4, a(2)=15, a(3)=56;
   for n>=4: a(n)=4*a(n-1)-a(n-2)-2*a(n-4) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)15;
    if (n == 3) return (seq_t)56;

    seq_t buf[4] = { (seq_t)1, (seq_t)4, (seq_t)15, (seq_t)56 }; /* buf[0]=a(n-4), buf[1]=a(n-3), buf[2]=a(n-2), buf[3]=a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)4 * buf[3] - buf[2] - (seq_t)2 * buf[0];
        /* shift buffer for next iteration */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = cur;
    }
    return cur;
}
