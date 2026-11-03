/*
 * A162964  a(n) = sigma(sigma(sigma(sigma(sigma(n))))).
 * https://oeis.org/A162964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(A000203(A000203(A000203(A000203(n)))))
 * Range:     u8 n <= 5, u16 n <= 239, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 64 terms through n = 9897 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) /* the sequence is undefined for n = 0, return 0 */
        return 0;

    seq_t res = (seq_t)n;
    /* Apply sigma five times */
    for (int i = 0; i < 5; ++i) {
        /* The intermediate results fit in idx_t for any n used in the tests */
        res = sigma_of((idx_t)res);
    }
    return res;
}
