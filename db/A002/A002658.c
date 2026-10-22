/*
 * A002658  a(0) = a(1) = 1; for n > 0, a(n+1) = a(n)*(a(0) + ... + a(n-1)) +
 *          a(n)*(a(n) + 1)/2.
 * https://oeis.org/A002658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, core, nice
 * Formula:   a(n+1) = a(n)*(a(0) + ... + a(n-1)) + a(n)*(a(n) + 1)/2, a(0) =
 *            a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   a002658_tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002658_tri.h"

seq_t a(idx_t n) {
    seq_t cur, sum, next;
    idx_t i;
    if (n <= 1)
        return (seq_t)1;
    cur = (seq_t)1;   /* a(1) */
    sum = (seq_t)1;   /* a(0) */
    for (i = 1; i < n; i++) {
        next = cur * sum + a002658_tri(cur);
        sum += cur;
        cur = next;
    }
    return cur;
}
