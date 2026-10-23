/*
 * A101660  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 12.
 * https://oeis.org/A101660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = [a(n)==2], a(2n+1) = 1 + [a(n)>0], a(0)=0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if ((n & 1) == 0)                  /* n is even: a(2n) */
        return (a(n >> 1) == (seq_t)2) ? (seq_t)1 : (seq_t)0;
    else                                /* n is odd: a(2n+1) */
        return 1 + (a(n >> 1) > (seq_t)0);
}
