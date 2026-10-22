/*
 * A005175  Number of rooted trees with 5 nodes of disjoint sets of labels
 *          with union {1..n}. If a node has an empty set of labels then it
 *          must have at least two children.
 * https://oeis.org/A005175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*S2(n,3) + 113*S2(n,4) + 625*S2(n,5), where S2 is the
 *            Stirling number of the second kind, computed by S2(i,k) =
 *            k*S2(i-1,k) + S2(i-1,k-1), S2(0,0) = 1 (equivalent to Layman's /
 *            McMorris-Zaslavsky formula)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t s[6];
    idx_t i;
    int k;
    for (k = 0; k < 6; k++) s[k] = 0;
    s[0] = 1;
    for (i = 1; i <= n; i++) {
        for (k = 5; k >= 1; k--) {
            s[k] = (seq_t)k * s[k] + s[k - 1];
        }
        s[0] = 0;
    }
    return (seq_t)3 * s[3] + (seq_t)113 * s[4] + (seq_t)625 * s[5];
}
