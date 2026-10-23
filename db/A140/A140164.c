/*
 * A140164  Binomial transform of [1, 1, 1, 1, -1, -1, 5, -11, 19, -29, 41,
 *          ...].
 * https://oeis.org/A140164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1; a(2)=2; a(3)=4; a(n)=6*n-16 for n>=4
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;
    return (seq_t)6 * (seq_t)n - (seq_t)16;
}
