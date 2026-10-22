/*
 * A000805  Permanent of a certain cyclic n X n (0,1) matrix.
 * https://oeis.org/A000805
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 96 + Sum_{i=1..15} b(i)*a(n-i) for n >= 21, where b = [2,
 *            2, 1, 0, -4, -18, -16, -12, -10, -4, 4, 3, 2, 2, 1]. Initial
 *            values are computed from the definition: a(n) = n! for n < 6,
 *            and for 6 <= n <= 20, a(n) = per(M) where M[i][j] = 1 iff (j -
 *            i) mod n is in {0..5}. This permanent is found...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   a805_perm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a805_perm.h"

static const int a805_b[15] = {2, 2, 1, 0, -4, -18, -16, -12, -10, -4, 4, 3, 2, 2, 1};

seq_t a(idx_t n) {
    seq_t w[16];
    idx_t k;
    int i;

    if (n < 6) {
        seq_t f = 1;
        for (k = 2; k <= n; k++) f *= (seq_t)k;
        return f;
    }
    if (n <= 20) {
        return (seq_t)a805_perm((unsigned)n);
    }

    /* circular buffer: w[k % 16] holds a(k) */
    for (k = 6; k < 21; k++) {
        w[k % 16] = (seq_t)a805_perm((unsigned)k);
    }

    for (k = 21; k <= n; k++) {
        seq_t pos = 96;
        seq_t neg = 0;
        for (i = 1; i <= 15; i++) {
            int c = a805_b[i - 1];
            seq_t t = w[(k - (idx_t)i) % 16];
            if (c > 0) {
                pos += t * (seq_t)c;
            } else if (c < 0) {
                neg += t * (seq_t)(-c);
            }
        }
        w[k % 16] = pos - neg;
    }

    return w[n % 16];
}
