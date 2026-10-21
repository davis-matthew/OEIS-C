/*
 * A094729  Number of connected ordered 2-element multiantichains on a labeled
 *          n-set.
 * https://oeis.org/A094729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4-3*2^n+3^n (for n>0); a(0)=0
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow2 = 2;
    seq_t pow3 = 3;
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= 2;
        pow3 *= 3;
    }
    seq_t res = pow3 + 4;
    res -= 3 * pow2;
    return res;
}
