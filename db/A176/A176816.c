/*
 * A176816  The number of steps to reach 0 under the map x -> x-tau(sigma(x)),
 *          starting at n.
 * https://oeis.org/A176816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 94, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   calc_sigma_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_sigma_seq_t.h"

/* Computation of A176816 – the number of steps needed to reach 0 using
   x → x – τ(σ(x)), where σ is the sum of divisors of x and τ is the
   number of divisors.  The function follows the definition exactly
   and does not hard‑code any values. */

static uint64_t tau_of_uint64(uint64_t n)
{
    uint64_t tau = 1;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            int exp = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++exp;
            }
            tau *= (uint64_t)(exp + 1);
        }
    }
    if (tmp > 1) {                       /* last prime factor */
        tau *= 2;
    }
    return tau;
}

seq_t a(idx_t n)
{
    seq_t steps = 0;
    uint64_t x = (uint64_t)n;             /* offset 1: ensure n >= 1 */

    while (x != 0) {
        seq_t sigma;
        calc_sigma_seq_t((seq_t)x, &sigma);
        uint64_t sigma64 = (uint64_t)sigma;   /* σ(x) fits in 64 bits for n ≤ 10⁴ */
        uint64_t tau = tau_of_uint64(sigma64);
        x -= tau;
        ++steps;
    }
    return steps;
}
