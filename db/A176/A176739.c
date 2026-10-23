/*
 * A176739  Expansion of 1/(1-2*x^2-4*x^3). (2,4)-Padovan sequence.
 * https://oeis.org/A176739
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) + 4*a(n-3) for n ≥ 3, with a(0)=1, a(1)=0,
 *            a(2)=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    seq_t buf[3];
    buf[0] = (seq_t)1;   /* a(0) */
    buf[1] = (seq_t)0;   /* a(1) */
    buf[2] = (seq_t)2;   /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* idxs for a(i-2) and a(i-3) in the circular buffer */
        seq_t prev2 = buf[(i + 1) % 3];  /* a(i-2) */
        seq_t prev3 = buf[i % 3];        /* a(i-3) */

        seq_t cur = (seq_t)2 * prev2 + (seq_t)4 * prev3;
        buf[i % 3] = cur;
    }

    return buf[n % 3];
}
