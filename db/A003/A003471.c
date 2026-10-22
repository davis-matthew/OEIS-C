/*
 * A003471  Number of permutations with no hits on 2 main diagonals.
 * https://oeis.org/A003471
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)*a(n-1) + 2*(n-d)*a(n-e), where (d, e) = (2, 4) if n
 *            even, (1, 2) if n odd; a(0) = 1, a(1) = a(2) = a(3) = 0
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t a4, a3, a2, a1, cur;
    idx_t i;

    if (n == 0) return (seq_t)1;
    if (n < 4) return (seq_t)0;

    a4 = (seq_t)1; /* a(i-4) */
    a3 = (seq_t)0; /* a(i-3) */
    a2 = (seq_t)0; /* a(i-2) */
    a1 = (seq_t)0; /* a(i-1) */
    cur = (seq_t)0;

    for (i = 4; i <= n; i++) {
        cur = (seq_t)(i - 1) * a1;
        if (i % 2 == 0) {
            cur += (seq_t)2 * (seq_t)(i - 2) * a4;
        } else {
            cur += (seq_t)2 * (seq_t)(i - 1) * a2;
        }
        a4 = a3;
        a3 = a2;
        a2 = a1;
        a1 = cur;
    }
    return cur;
}
