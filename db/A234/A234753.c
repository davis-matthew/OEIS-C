/*
 * A234753  Number of (n+1) X (1+1) 0..3 arrays with 2 X 2 edge jumps all no
 *          more than +1 in one of the clockwise or counterclockwise
 *          directions or both.
 * https://oeis.org/A234753
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) + 10*a(n-2) - 16*a(n-3) - 8*a(n-4) + 4*a(n-5) +
 *            a(n-6), with a(1)=110, a(2)=1014, a(3)=8968, a(4)=80010,
 *            a(5)=712722, a(6)=6350732
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base terms – stored in order a(1) … a(6) */
    const seq_t base[6] = { (seq_t)110, (seq_t)1014, (seq_t)8968,
                            (seq_t)80010, (seq_t)712722, (seq_t)6350732 };

    if (n < 1)
        return (seq_t)0;          /* out of bounds, not used by OEIS */
    if (n <= 6)
        return base[n - 1];

    /* Sliding window of the last six values: w[0]=a(k-1), w[1]=a(k-2), …, w[5]=a(k-6) */
    seq_t w[6] = { base[5], base[4], base[3],
                   base[2], base[1], base[0] };

    for (idx_t k = 7; k <= n; ++k)
    {
        /* recurrence computation */
        seq_t val = w[0] * (seq_t)8;
        val += w[1] * (seq_t)10;
        val -= w[2] * (seq_t)16;
        val -= w[3] * (seq_t)8;
        val += w[4] * (seq_t)4;
        val += w[5];

        /* shift window to keep the last six values */
        w[5] = w[4];
        w[4] = w[3];
        w[3] = w[2];
        w[2] = w[1];
        w[1] = w[0];
        w[0] = val;
    }

    return w[0];   /* w[0] now holds a(n) */
}
