/*
 * A025815  Expansion of 1/((1-x^2)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A025815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^2 + 19*n + 210 + (11*n+102)*(-1)^n) /264)
 * Range:     u8 none, u16 n <= 293, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn + (seq_t)19 * nn + (seq_t)210;
    seq_t alt = (seq_t)11 * nn + (seq_t)102;
    seq_t numerator = term;
    
    /* Add or subtract the alternating part depending on parity of n */
    if ((n & 1) == 0) {
        numerator += alt;          /* even n -> (-1)^n = 1 */
    } else {
        numerator -= alt;          /* odd n -> (-1)^n = -1 */
    }
    
    return numerator / (seq_t)264;
}
