/*
 * A084940  Heptagorials: n-th polygorial for k=7.
 * https://oeis.org/A084940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   2*a(n) = n*(5*n-3)*a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t mult = (seq_t)i * ((seq_t)5 * i - 3);
        mult = mult / 2;
        res = res * mult;
    }
    return res;
}
