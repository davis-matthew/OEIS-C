/*
 * A059204  Number of non-unimodal permutations of n items (i.e., those which
 *          do not simply go up for the first part and then down for the rest,
 *          but at some point go down then up).
 * https://oeis.org/A059204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! - ceil(2^(n-1))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n = 0, ceil(2^(n-1)) = 1 */

seq_t a(idx_t n) {
    /* compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* compute ceil(2^(n-1)) */
    seq_t pow2 = (seq_t)1;
    /* for n==0 the required value is 1 (ceil of 0.5) */
    for (idx_t i = 1; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    return fact - pow2;
}
