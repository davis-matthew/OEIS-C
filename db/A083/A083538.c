/*
 * A083538  a(n) = sigma(n)*sigma(n+1)/gcd(sigma(n+1), sigma(n))^2.
 * https://oeis.org/A083538
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n)*sigma(n+1)/gcd(sigma(n+1), sigma(n))^2
 * Range:     u8 n <= 13, u16 n <= 142, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sigma_of_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_of_idx.h"

/* Euclidean algorithm on seq_t.  Works for any sized unsigned type. */
static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, not used */

    seq_t sigma_n  = sigma_of_idx(n);
    seq_t sigma_n1 = sigma_of_idx(n + (idx_t)1);

    seq_t g  = gcd_seq_t(sigma_n, sigma_n1);
    seq_t g2 = g * g;

    return (sigma_n * sigma_n1) / g2;
}
