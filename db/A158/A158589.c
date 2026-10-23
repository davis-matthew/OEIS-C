/*
 * A158589  a(n) = 324*n^2 - 18.
 * https://oeis.org/A158589
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 324*n^2 - 18
 * Range:     u8 none, u16 n <= 14, u32 n <= 3640, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    seq_t val = (seq_t)324 * sn * sn;
    val -= (seq_t)18;
    return val;
}
