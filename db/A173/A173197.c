/*
 * A173197  a(0)=1, a(n)= 2+2^n/6+4*(-1)^n/3, n>0.
 * https://oeis.org/A173197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 + 2^n/6 + 4*(-1)^n/3, a(0)=1 Recurrence used: a(0)=1,
 *            a(1)=1, a(2)=4, a(3)=2 a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3), n >
 *            3
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)1;
    if (n == (idx_t)2) return (seq_t)4;
    if (n == (idx_t)3) return (seq_t)2;

    seq_t a1 = (seq_t)1;  /* a(n-3)  */
    seq_t a2 = (seq_t)4;  /* a(n-2)  */
    seq_t a3 = (seq_t)2;  /* a(n-1)  */
    for (idx_t i = (idx_t)4; i <= n; ++i) {
        /* compute a(i) = 2*a3 + a2 - 2*a1 */
        seq_t twice_a3 = a3 + a3;
        seq_t twice_a1 = a1 + a1;
        seq_t ai = twice_a3 + a2 - twice_a1;
        a1 = a2;
        a2 = a3;
        a3 = ai;
    }
    return a3; /* a(n) */
}
