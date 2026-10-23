/*
 * A130054  Inverse Moebius transform of A023900.
 * https://oeis.org/A130054
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = product over prime powers p^e‖n of (1 - (p-1)·e)
 * Range:     i8 n <= 130, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Offset starts at 1 – safety for n==0 */
    if (n == 0)
        return 0;

    uint64_t m = (uint64_t)n;
    seq_t res = 1;

    /* factorize m by trial division – fast enough for n ≤ 10 000 */
    for (uint64_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            uint64_t e = 0;
            while (m % p == 0) {
                m /= p;
                ++e;
            }
            seq_t factor = 1 - (seq_t)(p - 1) * (seq_t)e;
            res *= factor;
        }
    }

    /* remaining prime factor, if any */
    if (m > 1) {
        seq_t factor = 1 - (seq_t)(m - 1) * (seq_t)1;
        res *= factor;
    }

    return res;
}
