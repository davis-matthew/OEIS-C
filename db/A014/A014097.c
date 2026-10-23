/*
 * A014097  a(n) = a(n-1)+a(n-4).
 * https://oeis.org/A014097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-4) with a(1)=a(2)=a(3)=1, a(4)=5
 * Range:     u8 n <= 17, u16 n <= 34, u32 n <= 68, u64 n <= 137,
 *            u128 n <= 275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 3) return (seq_t)1;
    if (n == 4) return (seq_t)5;

    seq_t a1 = (seq_t)1;            /* a(i-4) */
    seq_t a2 = (seq_t)1;            /* a(i-3) */
    seq_t a3 = (seq_t)1;            /* a(i-2) */
    seq_t a4 = (seq_t)5;            /* a(i-1) */
    seq_t val;
    for (idx_t i = 5; i <= n; ++i) {
        val = a4 + a1;              /* a(i) = a(i-1)+a(i-4) */
        /* shift window forward */
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = val;
    }
    return a4;
}
