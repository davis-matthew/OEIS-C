/*
 * A003148  a(n+1) = a(n) + 2n*(2n+1)*a(n-1), with a(0) = a(1) = 1.
 * https://oeis.org/A003148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n+1) = a(n) + 2n*(2n+1)*a(n-1), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    seq_t prev = 1; /* a(0) */
    seq_t cur = 1;  /* a(1) */
    idx_t i;
    if (n == 0) return prev;
    for (i = 1; i < n; i++) {
        /* compute a(i+1) = a(i) + 2i*(2i+1)*a(i-1) */
        seq_t k = (seq_t)i;
        seq_t mult = (seq_t)2 * k * ((seq_t)2 * k + 1);
        seq_t next = cur + mult * prev;
        prev = cur;
        cur = next;
    }
    return cur;
}
