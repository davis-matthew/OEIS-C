/*
 * A129076  a(n) = sigma(sigma(sigma(sigma(n)))), where sigma(n) = sum of
 *          divisors of n.
 * https://oeis.org/A129076
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sigma(sigma(sigma(sigma(n))))
 * Range:     u8 n <= 11, u16 n <= 659, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 981 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) = sum of all positive divisors of n. */

#include <stdint.h>
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
    /* First sigma */
    seq_t x1 = sigma_of(n);
    /* Convert to idx_t for the next call */
    idx_t n1 = (idx_t)x1;

    /* Second sigma */
    seq_t x2 = sigma_of(n1);
    idx_t n2 = (idx_t)x2;

    /* Third sigma */
    seq_t x3 = sigma_of(n2);
    idx_t n3 = (idx_t)x3;

    /* Fourth sigma */
    seq_t x4 = sigma_of(n3);

    return x4;
}
