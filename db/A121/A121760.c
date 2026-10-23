/*
 * A121760  In decimal number system, take negative power of 10 at odd digits
 *          of n. Sequence gives numerators of result.
 * https://oeis.org/A121760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn
 * Formula:   a(n) = numerator of the rational sum Σ d(i) · 10^( (-1)^(d(i)) ·
 *            (i-1) ), where d(i) are decimal digits of n (least significant
 *            digit at i = 1)
 * Range:     u8 n <= 102, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"   // seq_t pow10(idx_t)
#include "../utils/mul_small.h" // seq_t mul_small(seq_t, unsigned int)

seq_t a(idx_t n)
{
    /* First pass: find M = max(i-1) for odd digits of n. */
    idx_t tmp = n;
    idx_t i = 1;
    idx_t M = 0;
    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 10);
        if (d & 1) {
            if ((i - 1) > M) M = i - 1;
        }
        tmp /= 10;
        ++i;
    }

    /* Second pass: compute numerator before reduction. */
    tmp = n;
    i = 1;
    seq_t numerator = 0;
    while (tmp > 0) {
        uint8_t d = (uint8_t)(tmp % 10);
        seq_t term;
        if (d & 1) {                      /* odd digit → negative exponent  */
            term = mul_small(pow10(M - (i - 1)), d);
        } else {                          /* even digit → positive exponent */
            term = mul_small(pow10(M + (i - 1)), d);
        }
        numerator += term;
        tmp /= 10;
        ++i;
    }

    /* Reduce the fraction by cancelling common factors of 2 and 5. */
    idx_t v2 = 0, v5 = 0;
    seq_t ntemp = numerator;

    /* Count factors 2 up to M. */
    while ((ntemp & (seq_t)1) == 0 && v2 < M) {
        ntemp >>= 1;
        ++v2;
    }

    /* Count factors 5 up to M. */
    while (ntemp % (seq_t)5 == 0 && v5 < M) {
        ntemp /= 5;
        ++v5;
    }

    /* Apply the reduction to the original numerator. */
    seq_t result = numerator;
    for (idx_t j = 0; j < v2; ++j) result >>= 1;      /* divide by 2 */
    for (idx_t j = 0; j < v5; ++j) result /= (seq_t)5; /* divide by 5 */

    return result;
}
