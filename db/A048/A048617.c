/*
 * A048617  a(n) = 2*(n!)^2.
 * https://oeis.org/A048617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n!)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n){
    seq_t f = 1;
    for(idx_t i = 2; i <= n; ++i){
        f *= (seq_t)i;
    }
    return (f * f) * (seq_t)2;
}
