/*
 * A083337  a(n) = 2*a(n-1) + 2*a(n-2); a(0)=0, a(1)=3.
 * https://oeis.org/A083337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-2), a(0) = 0, a(1) = 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)3;

    seq_t a0 = (seq_t)0;   /* a(0) */
    seq_t a1 = (seq_t)3;   /* a(1) */
    seq_t a2;
    const seq_t two = (seq_t)2;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = two * a1 + two * a0;  /* a(i) = 2*a(i-1) + 2*a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a1;  /* a(n) */
}
