/*
 * A232582  Number of (n+1) X (1+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally, with no adjacent elements equal.
 * https://oeis.org/A232582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-3) Initial terms: a(1) = 0, a(2)
 *            = 2, a(3) = 4 (Computed iteratively; works for all n ≥ 1)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 79, u128 n <= 158,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* safety for out-of-range n (not expected in tests) */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)4;

    seq_t a1 = (seq_t)0; /* a(n-3)   */
    seq_t a2 = (seq_t)2; /* a(n-2)   */
    seq_t a3 = (seq_t)4; /* a(n-1)   */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t next = 2 * a3 - a2 + a1; /* a(n)   */
        a1 = a2;
        a2 = a3;
        a3 = next;
    }
    return a3;
}
