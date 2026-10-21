/*
 * A051609  a(n) = (3*n+9)!!!/9!!!, related to A032031 ((3*n)!!! triple
 *          factorials).
 * https://oeis.org/A051609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((3*n+9)!!!)/(9!!!) = product of {12,15,18,…,3n+9}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        idx_t term = 12 + (idx_t)3 * i;
        prod *= (seq_t)term;
    }
    return prod;
}
