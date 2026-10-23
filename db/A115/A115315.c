/*
 * A115315  a(n) = floor(L^3*{phi^(3*n-2), phi^(3*n-1), phi^(3*n-2) +
 *          phi^(3*n-1)}) where L = (1 + sqrt(5))/(2*sqrt(5)) and phi = (1 +
 *          sqrt(5))/2.
 * https://oeis.org/A115315
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = floor(L^3*{phi^(3n-2), phi^(3n-1),
 *            phi^(3n-2)+phi^(3n-1)})
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial terms a(0)…a(15) */
    static const seq_t init[16] = {
        0, 0, 1, 2, 4, 6, 11, 17, 28, 46,
        75, 121, 197, 319, 516, 836
    };

    if (n < 16) return init[n];

    seq_t buf[16];
    for (int i = 0; i < 16; ++i) buf[i] = init[i];

    for (idx_t i = 16; i <= n; ++i)
    {
        idx_t i1  = (i - 1) & (idx_t)15;
        idx_t i2  = (i - 2) & (idx_t)15;
        idx_t i3  = (i - 3) & (idx_t)15;
        idx_t i4  = (i - 4) & (idx_t)15;
        idx_t i5  = (i - 5) & (idx_t)15;
        idx_t i6  = (i - 6) & (idx_t)15;
        idx_t i7  = (i - 7) & (idx_t)15;
        idx_t i8  = (i - 8) & (idx_t)15;
        idx_t i9  = (i - 9) & (idx_t)15;
        idx_t i10 = (i -10) & (idx_t)15;
        idx_t i12 = (i -12) & (idx_t)15;
        idx_t i13 = (i -13) & (idx_t)15;
        idx_t i14 = (i -14) & (idx_t)15;
        idx_t i16 = (i -16) & (idx_t)15;

        seq_t pos = (seq_t)2 * buf[i1] + buf[i2] + (seq_t)2 * buf[i5]
                    + (seq_t)3 * buf[i6] + (seq_t)2 * buf[i9]
                    + (seq_t)3 * buf[i10] + (seq_t)3 * buf[i14];

        seq_t neg = (seq_t)3 * buf[i3] + buf[i4] + (seq_t)2 * buf[i7]
                    + (seq_t)4 * buf[i8] + (seq_t)3 * buf[i12]
                    + buf[i13] + buf[i16];

        buf[i & (idx_t)15] = pos - neg;
    }

    return buf[n & (idx_t)15];
}
