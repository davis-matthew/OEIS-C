/*
 * A079537  a(n) = phi(2*n+1)*d(2*n+1) - sigma(2*n+1).
 * https://oeis.org/A079537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = phi(2*n+1)*d(2*n+1) - sigma(2*n+1)
 * Range:     u8 n <= 65, u16 n <= 3711, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/phi.h"
#include "../utils/sigma_of.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n) {
    idx_t m = 2*n + 1;          /* m = 2*n + 1 */
    seq_t phi_m   = phi(m);                    /* Euler's totient of m */
    seq_t sigma_m = sigma_of(m);               /* Sum of divisors of m */
    seq_t d_m     = (seq_t)divisor_count_u64(m);/* Number of divisors of m */
    return phi_m * d_m - sigma_m;
}
