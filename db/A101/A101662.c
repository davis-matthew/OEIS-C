/*
 * A101662  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 21.
 * https://oeis.org/A101662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0, if n == 0; a(2n) = 2 * [a(n) == 2]; a(2n+1) = 1 +
 *            [a(n) == 1]
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if ((n & 1) == 0) {
        seq_t prev = a(n >> 1);
        return 2 * (prev == 2);
    } else {
        seq_t prev = a(n >> 1);
        return 1 + (prev == 1);
    }
}
