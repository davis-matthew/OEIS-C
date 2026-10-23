/*
 * A017335  a(n) = (10*n + 5)^7.
 * https://oeis.org/A017335
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 5)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 56, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute the base (10*n + 5) in seq_t */
    seq_t base = (seq_t)10 * (seq_t)n + (seq_t)5;
    /* compute the 7th power by repeated multiplication */
    seq_t res = (seq_t)1;
    int i;
    for (i = 0; i < 7; ++i) {
        res *= base;
    }
    return res;
}
