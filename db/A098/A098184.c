/*
 * A098184  a(n) = 3a(n-1)+a(n-2)+a(n-3), a(0)=1, a(1)=1, a(2)=5.
 * https://oeis.org/A098184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 18, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 3*a(n-1) + a(n-2) + a(n-3)
   initial: a(0) = 1, a(1) = 1, a(2) = 5 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)5;

    seq_t a0 = (seq_t)1;   /* a(n-3)   */
    seq_t a1 = (seq_t)1;   /* a(n-2)   */
    seq_t a2 = (seq_t)5;   /* a(n-1)   */

    for (idx_t i = 3; i <= n; ++i) {
        seq_t next = (seq_t)3 * a2 + a1 + a0;
        a0 = a1;
        a1 = a2;
        a2 = next;
    }
    return a2;
}
