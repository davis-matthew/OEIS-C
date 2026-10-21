/*
 * A209295  Antidiagonal sums of the gcd(.,.) array A109004.
 * https://oeis.org/A209295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n} φ(d) * (n/d + 1), a(0)=0
 * Range:     u8 n <= 47, u16 n <= 3359, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A209295 (antidiagonal sums of the gcd array). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

/* Helper: compute Euler's totient for a 64‑bit integer.
   All intermediate results fit in 64 bits for the problem size. */
static seq_t phi_u64(idx_t n)
{
    uint64_t m = n;
    uint64_t phi = n;
    idx_t p = 2;
    while ((uint64_t)p * (uint64_t)p <= m) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            phi -= phi / p;
        }
        ++p;
    }
    if (m > 1) phi -= phi / m;
    return (seq_t)phi;
}

/* Main sequence function.  Handles the offset a(0)=0 automatically. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t sum = 0;
    idx_t root = (idx_t) sqrt((double)n);          /* floor(sqrt(n)) */

    for (idx_t d = 1; d <= root; ++d) {
        if (n % d == 0) {
            idx_t q = n / d;

            /* divisor d */
            seq_t phi_d = phi_u64(d);
            sum += phi_d * ((seq_t)q + 1);

            /* complementary divisor q, if different */
            if (d != q) {
                seq_t phi_q = phi_u64(q);
                sum += phi_q * ((seq_t)d + 1);
            }
        }
    }
    return sum;
}
