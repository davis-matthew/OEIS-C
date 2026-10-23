/*
 * A101501  Number of walks between adjacent nodes on C_5 tensor J_2.
 * https://oeis.org/A101501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(2)=0; a(n)=2*a(n-1)+12*a(n-2)-16*a(n-3) for
 *            n>=3
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 17, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)0;

    seq_t a0 = (seq_t)0;   /* a(n-3)    */
    seq_t a1 = (seq_t)1;   /* a(n-2)    */
    seq_t a2 = (seq_t)0;   /* a(n-1)    */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = 2*a2 + 12*a1 - 16*a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
