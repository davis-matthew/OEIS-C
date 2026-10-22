/*
 * A003124  One of the basic cycles in the x->3x-1 (x odd) or x/2 (x even)
 *          problem.
 * https://oeis.org/A003124
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Cycle in x->3x-1 (x odd) or x/2 (x even): a(0) = 17, a(n) =
 *            f(a(n-1)) where f(x) = 3x-1 if x odd, x/2 if x even
 * Range:     u8 n <= 13, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = 17;
    for (idx_t i = 0; i < n; i++) {
        x = (x & 1) ? 3 * x - 1 : x >> 1;
    }
    return x;
}
