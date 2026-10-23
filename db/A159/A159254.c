/*
 * A159254  Numbers whose square ends with 49.
 * https://oeis.org/A159254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = 25*n - 7 when n is even, a(n) = 25*n - 18 when n is odd
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* No assumption about increasing calls: simple O(1) calculation. */
    if (n == 0) {
        /* behavior below offset does not matter; return 0 for safety */
        return (seq_t)0;
    }
    seq_t val = (seq_t)25 * (seq_t)n;
    if ((n & 1) == 0) {
        /* even n */
        val -= (seq_t)7;
    } else {
        /* odd n */
        val -= (seq_t)18;
    }
    return val;
}
