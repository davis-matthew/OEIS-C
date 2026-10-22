/*
 * A006531  Semiorders on n elements.
 * https://oeis.org/A006531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = Sum_{k=1..n} S(n,k) * k! * M(k-1), a(0) = 1, where S =
 *            Stirling2 and M = Motzkin numbers A001006. T(n,k) = k!*S(n,k) is
 *            built row by row with T(n,k) = k*(T(n-1,k) + T(n-1,k-1)), and
 *            M(m) = M(m-1) + Sum_{i=0..m-2} M(i)*M(m-2-i)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    seq_t *t = alloc_zero(n + 1);
    seq_t *m = alloc_zero(n + 1);

    /* Ordered set partitions: t[k] = k! * S(i,k) */
    t[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        for (idx_t k = i; k >= 1; k--) {
            t[k] = (t[k] + t[k - 1]) * (seq_t)k;
        }
        t[0] = 0;
    }

    /* Motzkin numbers M(0..n-1) */
    m[0] = 1;
    if (n > 1) m[1] = 1;
    for (idx_t j = 2; j < n; j++) {
        seq_t s = m[j - 1];
        for (idx_t i = 0; i + 2 <= j; i++) {
            s += m[i] * m[j - 2 - i];
        }
        m[j] = s;
    }

    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        result += t[k] * m[k - 1];
    }

    release(t, n + 1);
    release(m, n + 1);
    return result;
}
