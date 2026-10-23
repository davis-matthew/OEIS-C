/*
 * A171464  a(n) = 0+1+2+...+n in lunar arithmetic in base 4 written in base
 *          10.
 * https://oeis.org/A171464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{k≥0} min(3, ⌊n/4^k⌋) * 4^k
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    seq_t pow = 1;
    idx_t tmp = n;
    while (tmp > 0) {
        seq_t m = (tmp >= 3) ? (seq_t)3 : (seq_t)tmp;
        ans += m * pow;
        pow *= 4;
        tmp /= 4;
    }
    return ans;
}
