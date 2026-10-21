/*
 * A081915  a(n) = 4^n*(n^3 - 3*n^2 + 2*n + 384)/384.
 * https://oeis.org/A081915
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n-1) - 96*a(n-2) + 256*a(n-3) - 256*a(n-4), a(0)=1,
 *            a(1)=4, a(2)=16, a(3)=65
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 4;
    if (n == 2) return 16;
    if (n == 3) return 65;

    seq_t a0 = 1, a1 = 4, a2 = 16, a3 = 65, res = 0;
    for (idx_t i = 4; i <= n; ++i) {
        res = 16 * a3 - 96 * a2 + 256 * a1 - 256 * a0;
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = res;
    }
    return res;
}
