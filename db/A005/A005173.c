/*
 * A005173  Number of rooted trees with 3 nodes of disjoint sets of labels
 *          with union {1..n}. If a node has an empty set of labels then it
 *          must have at least two children.
 * https://oeis.org/A005173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 3^n - 2^(n+1), a(1) = 0 (equivalently a(n) =
 *            (3^(n+1) - 2^(n+3) + 7)/2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t r = 0;
    seq_t p3 = 3;
    seq_t p2 = 2;
    idx_t k;
    for (k = 2; k <= n; k++) {
        p3 *= 3;
        p2 *= 2;
        r += p3 - (p2 + p2);
    }
    return r;
}
