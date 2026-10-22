/*
 * A002594  a(n) = n^2*(16*n^4-20*n^2+7)/3.
 * https://oeis.org/A002594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(16*n^4-20*n^2+7)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 30, u64 n <= 1229,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    seq_t inner = 16 * n4 - 20 * n2 + 7;
    if (n % 3 == 0)
        return (n2 / 3) * inner;
    /* n^2 == 1 (mod 3), so 16 - 20 + 7 == 0 (mod 3): inner is divisible by 3 */
    return n2 * (inner / 3);
}
