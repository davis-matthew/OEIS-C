/*
 * A070584  a(n) = n^4 mod 41.
 * https://oeis.org/A070584
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 41
 * Range:     u8 n <= 3, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)(n % 41);          /* reduce n modulo 41 */
    seq_t r = m * m;                    /* m^2 */
    r = r % 41;                         /* (m^2) mod 41 */
    r = r * r;                          /* (m^2)^2 = m^4 */
    r = r % 41;                         /* full result modulo 41 */
    return r;
}
