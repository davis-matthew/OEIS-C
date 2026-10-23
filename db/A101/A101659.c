/*
 * A101659  Fixed point of morphism 0 -> 01, 1 -> 02, 2 -> 11.
 * https://oeis.org/A101659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2n) = [a(n) == 2], a(2n+1) = 1 + [a(n) == 1] (offset 0)
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
    if ((n & 1) == 0) {           /* even n  */
        seq_t v = a(n >> 1);
        return (v == 2) ? 1 : 0;
    } else {                      /* odd n   */
        seq_t v = a((n - 1) >> 1);
        return 1 + ((v == 1) ? 1 : 0);  /* 1 or 2 */
    }
}
