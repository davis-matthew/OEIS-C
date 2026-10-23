/*
 * A143238  a(n) = A000203(n) * A024916(n).
 * https://oeis.org/A143238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 35, u32 n <= 1199, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = A000203(n) * A024916(n)
 * where A000203(n) = sum_{d|n} d  (sum of divisors)
 * and A024916(n) = sum_{k=1..n} k * floor(n / k)
 * (OEIS A024916: "Sum_{k=1..n} k*floor(n/k)")
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* 1. compute sigma(n) = sum of divisors of n */
    seq_t sigma = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sigma += (seq_t)d;
            idx_t q = n / d;
            if (q != d) sigma += (seq_t)q;
        }
    }

    /* 2. compute g(n) = sum_{k=1..n} k * floor(n/k)
     *    using the standard O(sqrt(n)) trick
     */
    seq_t g = (seq_t)0;
    for (idx_t l = 1; l <= n; ) {
        idx_t t = n / l;               /* floor(n/l) */
        idx_t r = n / t;               /* last k with same floor */
        seq_t cnt = (seq_t)(r - l + 1);
        seq_t sum_lr = ((seq_t)l + (seq_t)r) * cnt / 2; /* sum l..r */
        g += sum_lr * (seq_t)t;
        l = r + 1;
    }

    return sigma * g;
}
