/*
 * A005416  Vertex diagrams of order 2n.
 * https://oeis.org/A005416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (2*n - 1) * A000698(n) for n >= 1, a(0) = 1, where b(n) =
 *            A000698(n) satisfies b(0) = 1, b(n) = (2n-1)!! - Sum_{k=1..n-1}
 *            (2k-1)!! * b(n-k) (from B(x) = 2 - 1/S(x))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/grow_zero.h"

static seq_t *s_dfact = NULL; /* s_dfact[k] = (2k-1)!! */
static seq_t *s_b = NULL;     /* s_b[k] = A000698(k) */
static idx_t s_len = 0;

static void ensure(idx_t n) {
    idx_t need = n + 1;
    if (need <= s_len) return;
    idx_t newlen = s_len ? s_len : 16;
    while (newlen < need) newlen *= 2;
    s_dfact = grow_zero(s_dfact, s_len, newlen);
    s_b = grow_zero(s_b, s_len, newlen);
    idx_t start = s_len;
    if (start == 0) {
        s_dfact[0] = 1;
        s_b[0] = 1;
        start = 1;
    }
    for (idx_t m = start; m < newlen; m++) {
        s_dfact[m] = s_dfact[m - 1] * (seq_t)(2 * m - 1);
        seq_t sum = 0;
        for (idx_t k = 1; k < m; k++) {
            sum += s_dfact[k] * s_b[m - k];
        }
        s_b[m] = s_dfact[m] - sum;
    }
    s_len = newlen;
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    ensure(n);
    return (seq_t)(2 * n - 1) * s_b[n];
}
