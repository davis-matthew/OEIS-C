/*
 * A189529  a(n) = n + [n*r/t] + [n*s/t]; r=1, s=sqrt(3)-sqrt(2),
 *          t=sqrt(3)+sqrt(2).
 * https://oeis.org/A189529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + ⌊ n / (√3 + √2) ⌋ + ⌊ n / (5 + 2√6) ⌋ (with offset 1,
 *            so a(1) = 1)
 * Range:     u8 n <= 180, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_div_gamma, floor_div_tau, gamma_le, tau_le
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_div_gamma.h"
#include "../utils/floor_div_tau.h"
#include "../utils/gamma_le.h"
#include "../utils/tau_le.h"

/* Main sequence function required by the framework. */
seq_t a(idx_t n)
{
    seq_t res = (seq_t)n;
    res += (seq_t)floor_div_tau(n);
    res += (seq_t)floor_div_gamma(n);
    return res;
}
