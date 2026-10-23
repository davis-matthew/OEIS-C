/*
 * A016806  a(n) = (4n)^6.
 * https://oeis.org/A016806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n)^6
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 10, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute base = 4 * n */
    seq_t base = (seq_t)4 * (seq_t)n;

    /* Compute base^6 efficiently:
       b2 = base^2
       b4 = base^4 = b2^2
       res = b4 * b2 = base^6
    */
    seq_t b2 = base * base;
    seq_t b4 = b2 * b2;
    seq_t res = b4 * b2;

    return res;
}
