/*
 * A038999  Sublattices of index n in generic 12-dimensional lattice.
 * https://oeis.org/A038999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} ∏_{k=1..11} (p^(e+k)−1)/(p^k−1)
 *            (multiplicative, a(1)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 51, u128 n <= 2735,
 *            big any n
 * Helpers:   factor_u64, gcd, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_seq.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Factor n into primes and exponents */
    uint64_t primes[64], exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;

    for (size_t f = 0; f < cnt; ++f) {
        seq_t p   = (seq_t)primes[f];
        uint64_t e = exps[f];

        seq_t num[11];
        seq_t den[11];

        /* Build the 11 numerators and denominators */
        for (uint64_t k = 1; k <= 11; ++k) {
            num[k - 1] = pow_seq(p, e + k);
            num[k - 1] -= 1;
            den[k - 1] = pow_seq(p, k);
            den[k - 1] -= 1;
        }

        /* Reduce numerators and denominators by GCDs */
        for (int i = 0; i < 11; ++i) {
            while (den[i] > 1) {
                bool progress = false;
                for (int j = 0; j < 11; ++j) {
                    seq_t g = gcd(den[i], num[j]);
                    if (g > 1) {
                        den[i] /= g;
                        num[j] /= g;
                        progress = true;
                        if (den[i] == 1) break;
                    }
                }
                if (!progress) break;          /* should not happen for valid data */
            }
        }

        /* Multiply remaining numerators; denominators should be 1 */
        for (int j = 0; j < 11; ++j) {
            result *= num[j];
        }
    }

    return result;
}
