/*
 * A233272  a(n) = n + 1 + number of nonleading zeros in binary representation
 *          of n (A080791).
 * https://oeis.org/A233272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 + floor(n/2) + a(floor(n/2)), with a(0)=1. Equivalent
 *            to a(n) = n + A080791(n) + 1
 * Range:     u8 n <= 253, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t ans = 1;            /* a(0) */
    while (n) {
        ans += ((seq_t)n) / 2 + 1;
        n >>= 1;
    }
    return ans;
}
