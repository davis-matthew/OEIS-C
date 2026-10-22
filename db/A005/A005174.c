/*
 * A005174  Number of rooted trees with 4 nodes of disjoint sets of labels
 *          with union {1..n}. If a node has an empty set of labels then it
 *          must have at least two children.
 * https://oeis.org/A005174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8/3)*(4^n - 4) - 9*3^n + 11*2^n + 5 = 32*Sum_{k=0..n-2}
 *            4^k - 9*3^n + 11*2^n + 5 (no division, safe under wraparound)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t s = 0, p4 = 1, p3 = 3, p2 = 2;
    idx_t k;
    for (k = 1; k < n; k++) {
        s += p4;
        p4 *= 4;
        p3 *= 3;
        p2 *= 2;
    }
    return (seq_t)32 * s + (seq_t)11 * p2 + (seq_t)5 - (seq_t)9 * p3;
}
