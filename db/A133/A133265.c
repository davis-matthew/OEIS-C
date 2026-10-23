/*
 * A133265  Diagonal of the A135356 triangle.
 * https://oeis.org/A133265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3 + (n-1)*(-1)^(n+1))/2 (equivalent to a(n)=2 for even
 *            n, a(n)=n+1 for odd n)
 * Range:     u8 n <= 254, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if ((n & 1ULL) == 0)
        return (seq_t)2;
    else
        return ((seq_t)n) + 1;
}
