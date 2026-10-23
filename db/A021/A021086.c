/*
 * A021086  Decimal expansion of 1/82.
 * https://oeis.org/A021086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(0)=0; for n>=1, a(n) = {1,2,1,9,5}[(n-1)%5]
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    static const seq_t cycle[5] = {1, 2, 1, 9, 5};
    idx_t pos = (n - 1) % 5;
    return cycle[pos];
}
