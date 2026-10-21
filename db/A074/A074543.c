/*
 * A074543  a(n) = 2^n + 6^n + 9^n.
 * https://oeis.org/A074543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n + 6^n + 9^n (Recurrence: a(n) = 17*a(n-1) - 84*a(n-2)
 *            + 108*a(n-3), a(0)=3, a(1)=17, a(2)=121)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)3;
    if (n == (idx_t)1)
        return (seq_t)17;
    if (n == (idx_t)2)
        return (seq_t)121;

    seq_t a0 = (seq_t)3;  /* a(n-3)  */
    seq_t a1 = (seq_t)17; /* a(n-2)  */
    seq_t a2 = (seq_t)121;/* a(n-1)  */
    seq_t a  = (seq_t)0;

    for (idx_t k = (idx_t)3; k <= n; ++k) {
        /* a = 17*a2 - 84*a1 + 108*a0 */
        a = (seq_t)17 * a2;
        a -= (seq_t)84 * a1;
        a += (seq_t)108 * a0;

        /* shift for next iteration */
        a0 = a1;
        a1 = a2;
        a2 = a;
    }
    return a2;
}
