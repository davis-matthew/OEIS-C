/*
 * A006591  a(n) = Sum_{k=1..n} nearest integer to n/k (if n/k is midway
 *          between two numbers take the smaller).
 * https://oeis.org/A006591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} nearest_int(n/k), rounding .5 down
 * Range:     u8 n <= 55, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t n_seq = (seq_t)n;
    for (idx_t k = 1; k <= n; k++) {
        seq_t quotient = n_seq / (seq_t)k;
        seq_t remainder = n_seq % (seq_t)k;
        /* Round to nearest, with tie-breaking toward smaller */
        if (2 * remainder > (seq_t)k) {
            result += quotient + 1;
        } else {
            result += quotient;
        }
    }
    return result;
}
