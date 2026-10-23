/*
 * A029906  Zack's sequence (the pattern is evident).
 * https://oeis.org/A029906
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-4) - a(n-8) Initial values:
 *            a(0)=0,a(1)=1,a(2)=1,a(3)=2,a(4)=0,a(5)=2,a(6)=2,a(7)=4
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* initial 8 terms */
    static const seq_t init[8] = {0, 1, 1, 2, 0, 2, 2, 4};

    if (n < 8)
        return init[n];

    /* cyclic buffer of the last 8 computed values */
    seq_t buf[8];
    for (idx_t i = 0; i < 8; ++i)
        buf[i] = init[i];

    seq_t val = 0;          /* will hold a(n) for the final iteration */
    for (idx_t cur = 8; cur <= n; ++cur) {
        idx_t i4  = (cur - 4) & 7;
        idx_t i8  = (cur - 8) & 7;
        val = 2 * buf[i4] - buf[i8];
        buf[cur & 7] = val;
    }
    return val;
}
