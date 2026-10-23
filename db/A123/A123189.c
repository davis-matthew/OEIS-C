/*
 * A123189  a(0) = 0, a(1) = 1, a(2)=1; a(n) = 3a(n-1) + 3a(n-2) - 4a(n-3) for
 *          n >= 3.
 * https://oeis.org/A123189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=1, a(2)=1; a(n) = 3*a(n-1) + 3*a(n-2) - 4*a(n-3)
 *            for n >= 3
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1 || n == 2) return (seq_t)1;

    /* iterate from 3 up to n */
    seq_t a0 = (seq_t)0;   /* a(n-3) */
    seq_t a1 = (seq_t)1;   /* a(n-2) */
    seq_t a2 = (seq_t)1;   /* a(n-1) */
    seq_t t;

    for (idx_t i = 3; i <= n; ++i) {
        t = (seq_t)3 * a2 + (seq_t)3 * a1 - (seq_t)4 * a0;
        a0 = a1;
        a1 = a2;
        a2 = t;
    }
    return a2;
}
