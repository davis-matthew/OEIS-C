/*
 * A032821  Numbers whose set of base-9 digits is {1,4}.
 * https://oeis.org/A032821
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(1)=1, a(2)=4; If n is odd: a(n)=9*a(floor(n/2))+1; If n is
 *            even: a(n)=9*a(floor((n-1)/2))+4
 * Range:     u8 n <= 10, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n & 1) {
        return (seq_t)9 * a(n >> 1) + (seq_t)1;
    } else {
        return (seq_t)9 * a((n - 1) >> 1) + (seq_t)4;
    }
}
