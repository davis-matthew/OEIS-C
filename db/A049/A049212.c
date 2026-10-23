/*
 * A049212  a(n) = -Product_{k=0..n} (10*k - 1); deca-factorial numbers.
 * https://oeis.org/A049212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{k=1..n} (10*k - 1), with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i) {
        seq_t term = (seq_t)10 * (seq_t)i - (seq_t)1;
        prod = prod * term;
    }
    return prod;
}
