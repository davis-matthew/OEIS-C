/*
 * A108784  Difference between A107757 and A107755.
 * https://oeis.org/A108784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^(popcount(n)+1) i.e. a(n)=1 if the binary
 *            representation of n has an odd number of 1's, a(n)=-1 otherwise.
 *            This matches the OEIS definition of A108784
 * Range:     i8 n <= 120, i16 n <= 120, i32 n <= 120, i64 n <= 120,
 *            i128 n <= 120, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;
    /* parity of popcount: 1 if odd number of 1-bits, 0 if even */
    unsigned parity = 0;
    while (x) {
        parity ^= 1;
        x &= x - 1;          /* remove lowest set bit */
    }
    return parity ? (seq_t)1 : (seq_t)-1;
}
