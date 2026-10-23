/*
 * A035272  One third of deca-factorial numbers.
 * https://oeis.org/A035272
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Π_{j=2..n} (10*j-7) (a(1)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    for (idx_t j = 2; j <= n; ++j) {
        result *= ((seq_t)10 * (seq_t)j - (seq_t)7);
    }
    return result;
}
