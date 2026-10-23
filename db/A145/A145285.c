/*
 * A145285  a(n) is the number of monomials in the n-th power of polynomial
 *          x^4-x^3-x^2-x-1.
 * https://oeis.org/A145285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + 1 for n > 8; a(1)=5, a(2)=8, a(3)=12, a(4)=16,
 *            a(5)=20, a(6)=25, a(7)=28, a(8)=32
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 8) {
        static const seq_t table[8] = {5, 8, 12, 16, 20, 25, 28, 32};
        return table[n - 1];
    }
    return (seq_t)n * (seq_t)4 + 1;
}
