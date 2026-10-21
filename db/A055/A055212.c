/*
 * A055212  Number of composite divisors of n.
 * https://oeis.org/A055212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = tau(n) - omega(n) - 1 (number of composite divisors of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_tau_omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_tau_omega.h"

seq_t a(idx_t n)
{
    seq_t tau, omega;
    count_tau_omega(n, &tau, &omega);
    return tau - omega - (seq_t)1;
}
