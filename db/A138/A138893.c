/*
 * A138893  A generalized Chamberland function.
 * https://oeis.org/A138893
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = * 8*(n/4) if n mod 4 == 0 * 11+36*(n/4) if n mod 4 == 1 *
 *            36+64*(n/4) if n mod 4 == 2 * 29+36*(n/4) if n mod 4 == 3
 * Range:     u8 n <= 17, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* integer division by 4 (right shift) */
    idx_t q = n >> 2;
    /* convert quotient to seq_t for arithmetic */
    seq_t qS = (seq_t)q;

    switch (n & 3U) {
        case 0: /* n = 4k */
            return 8 * qS;
        case 1: /* n = 4k+1 */
            return 11 + 36 * qS;
        case 2: /* n = 4k+2 */
            return 36 + 64 * qS;
        case 3: /* n = 4k+3 */
            return 29 + 36 * qS;
    }
    /* unreachable return, but keep compiler happy */
    return 0;
}
