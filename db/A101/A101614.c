/*
 * A101614  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 10.
 * https://oeis.org/A101614
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = [a(n)==2]; a(2n+1) = (a(n)+1) mod 3; Initial values:
 *            a(0)=0, a(1)=1
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n & 1) {
        idx_t m = n >> 1;                /* n = 2m+1 */
        return (a(m) + 1) % 3;
    } else {
        idx_t m = n >> 1;                /* n = 2m */
        return (a(m) == 2) ? 1 : 0;
    }
}
