/*
 * A079537  a(n) = phi(2*n+1)*d(2*n+1) - sigma(2*n+1).
 * https://oeis.org/A079537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(2*n+1)*d(2*n+1) - sigma(2*n+1)
 * Range:     u8 n <= 65, u16 n <= 3711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9639 (2026-11-01)
 * Helpers:   num_divisors, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/num_divisors.h"
#include "../utils/totient.h"

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

seq_t a(idx_t n) {
    idx_t m = 2*n + 1;          /* m = 2*n + 1 */
    seq_t phi_m   = totient(m);                    /* Euler's totient of m */
    seq_t sigma_m = sigma_of(m);               /* Sum of divisors of m */
    seq_t d_m     = (seq_t)num_divisors(m);/* Number of divisors of m */
    return phi_m * d_m - sigma_m;
}
