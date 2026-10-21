/*
 * A051535  a(n)=2*a(n-1), except every tenth time you multiply by 1000/512
 *          instead of by 2.
 * https://oeis.org/A051535
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1000^(n/10) * 2^(n%10)
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t q = n / 10;
    idx_t r = n % 10;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < q; ++i) result *= (seq_t)1000;
    for (idx_t i = 0; i < r; ++i) result *= (seq_t)2;
    return result;
}
