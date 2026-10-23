/*
 * A022409  a(n) = a(n-1) + a(n-2) + 1, with a(0)=3, a(1)=10.
 * https://oeis.org/A022409
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 3, a(1) = 10
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 3;
    if (n == 1) return 10;
    seq_t a0 = 3;
    seq_t a1 = 10;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t tmp = a1 + a0 + 1;
        a0 = a1;
        a1 = tmp;
    }
    return a1;
}
