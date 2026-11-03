/*
 * A082901  a(n) = A082895(n)-A000203(n); the distance from sigma(n) to that
 *          multiple of n which is closest to sigma(n), positive terms for
 *          cases where the closest multiple is after sigma(n), and negative
 *          terms where it is before sigma(n). In case of ties, a positive
 *          term is selected.
 * https://oeis.org/A082901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, look
 * Formula:   a(n) = n*floor[(floor(n/2)+sigma(n))/n] - sigma(n)
 * Range:     i8 n <= 59, i16 n <= 65536, i32 n <= 65537, i64 n <= 65537,
 *            i128 n <= 65537, big any n
 * Validated: standard, pass, 100 terms through n = 9763 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* local helper: divisor sum σ(n) for 64‑bit n */
static seq_t sigma_of_num(idx_t n)
{
    seq_t sum = 1;
    idx_t rem = n;
    for (idx_t p = 2; p * p <= rem; ++p) {
        if (rem % p == 0) {
            idx_t power = p;
            seq_t term = 1 + (seq_t)power;
            rem /= p;
            while (rem % p == 0) {
                rem /= p;
                power *= p;
                term += (seq_t)power;
            }
            sum *= term;
        }
    }
    if (rem > 1) {
        /* remaining prime factor */
        sum *= (1 + (seq_t)rem);
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* compute σ(n) */
    seq_t sigma = sigma_of_num(n);

    /* compute k = floor[(σ(n) + floor(n/2)) / n] */
    seq_t k = (sigma + (seq_t)(n / 2)) / (seq_t)n;

    /* a(n) = n*k - σ(n) */
    return (seq_t)n * k - sigma;
}
