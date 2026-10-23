/*
 * A139268  Twice nonagonal numbers (or twice 9-gonal numbers): a(n) =
 *          n*(7*n-5).
 * https://oeis.org/A139268
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(7*n-5)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    if (sn == 0) {
        return 0;
    }
    return sn * (sn * 7 - 5);
}
