/*
 * A070636  a(n) = n^6 mod 13.
 * https://oeis.org/A070636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^6 mod 13
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nm = (seq_t)n % 13;      /* reduce n modulo 13 */
    seq_t res = 1;                /* start with 1 */
    for (unsigned i = 0; i < 6; ++i) {
        res = (res * nm) % 13;    /* multiply and reduce */
    }
    return res;
}
