/*
 * A197354  a(n) = Sum_{k>=0} A030308(n,k)*(2k+1).
 * https://oeis.org/A197354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∑_{k≥0} A030308(n,k)·(2k+1)
 * Range:     u8 n <= 8192, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    for (idx_t i = 1; i <= n; ++i) {
        int64_t delta = 0;
        idx_t power = 1;   /* 2^k */
        int k = 0;
        while (power <= i) {
            /* power divides i  ⇔  all lower k bits of i are zero */
            if ((i & (power - 1)) == 0) {
                idx_t div = i / power;      /* i / 2^k  */
                int sign = (div & 1) ? 1 : -1;   /* +1 if odd, –1 if even */
                delta += (int64_t)sign * (2 * k + 1);
            }
            ++k;
            /* avoid overflow when shifting a very large power */
            if (power > (i >> 1)) break;
            power <<= 1;
        }
        if (delta >= 0) {
            result += (seq_t)delta;
        } else {
            result -= (seq_t)(-delta);
        }
    }
    return result;
}
