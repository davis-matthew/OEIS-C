/*
 * A000704  Number of degree-n even permutations of order dividing 2.
 * https://oeis.org/A000704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = E(n), where E(n) = E(n-1) + (n-1)*O(n-2), O(n) = O(n-1) +
 *            (n-1)*E(n-2), E(0) = 1, O(0) = 0, E(1) = 1, O(1) = 0 (E/O =
 *            involutions with an even/odd number of 2-cycles; a(n) =
 *            Sum_{i=0..floor(n/4)} C(n,4i)*(4i-1)!!)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t e2 = 1, o2 = 0; /* E(k-2), O(k-2) */
    seq_t e1 = 1, o1 = 0; /* E(k-1), O(k-1) */
    if (n <= 1) return 1;
    for (idx_t k = 2; k <= n; k++) {
        seq_t m = (seq_t)(k - 1);
        seq_t e = e1 + m * o2;
        seq_t o = o1 + m * e2;
        e2 = e1; o2 = o1;
        e1 = e;  o1 = o;
    }
    return e1;
}
