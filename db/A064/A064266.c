/*
 * A064266  Lune of Jan 01 in Julian calendar for a year with Golden Number n.
 * https://oeis.org/A064266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = ((n*11 + 28) mod 30); if the result is 0, return 30
 * Range:     u8 n <= 19, u16 n <= 19, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the golden-number adjustment modulo 30 */
    idx_t t = (n * 11ULL + 28ULL) % 30ULL;
    seq_t val = (seq_t)t;
    if (val == 0)
        val = 30;
    return val;
}
