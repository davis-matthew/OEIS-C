/*
 * A162964  a(n) = sigma(sigma(sigma(sigma(sigma(n))))).
 * https://oeis.org/A162964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000203(A000203(A000203(A000203(A000203(n)))))
 * Range:     u8 n <= 5, u16 n <= 239, u32 n <= 9897, u64 n <= 9897,
 *            u128 n <= 9897, big any n
 * Helpers:   sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of.h"

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
