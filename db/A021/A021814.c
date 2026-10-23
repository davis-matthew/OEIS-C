/*
 * A021814  Expansion of 1/((1-x)(1-4x)(1-6x)(1-8x)).
 * https://oeis.org/A021814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 122*a(n-2) + 296*a(n-3) - 192*a(n-4) with
 *            a(0)=1, a(1)=19, a(2)=239, a(3)=2519
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)19;
    if (n == (idx_t)2) return (seq_t)239;
    if (n == (idx_t)3) return (seq_t)2519;

    /* previous four values: t0=a(n-4), t1=a(n-3), t2=a(n-2), t3=a(n-1) */
    seq_t t0 = (seq_t)1;        /* a0 */
    seq_t t1 = (seq_t)19;       /* a1 */
    seq_t t2 = (seq_t)239;      /* a2 */
    seq_t t3 = (seq_t)2519;     /* a3 */

    for (idx_t i = (idx_t)4; i <= n; ++i)
    {
        seq_t cur = (seq_t)19 * t3
                  - (seq_t)122 * t2
                  + (seq_t)296 * t1
                  - (seq_t)192 * t0;

        /* shift the window for the next iteration */
        t0 = t1;
        t1 = t2;
        t2 = t3;
        t3 = cur;
    }

    return t3; /* t3 now holds a(n) */
}
