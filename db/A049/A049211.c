/*
 * A049211  a(n) = Product_{k=1..n} (9*k - 1); 9-factorial numbers.
 * https://oeis.org/A049211
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=1..n} (9*k-1), a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        res *= ((seq_t)9 * (seq_t)k - 1);
    }
    return res;
}
