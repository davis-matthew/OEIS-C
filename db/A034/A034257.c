/*
 * A034257  Maximal discrete supergroups of Gamma_0(n).
 * https://oeis.org/A034257
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = ∏_{p^e‖n} (3 + 2 p (e-1))
 *                      if n = 1 -> 1
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;
    // Factor n. 64 is more than enough for any 64‑bit integer.
    uint64_t primes[64], exps[64];
    size_t pcnt = 0;
    factor_u64(n, primes, exps, &pcnt);
    seq_t result = 1;
    for (size_t i = 0; i < pcnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t e = (seq_t)exps[i];
        seq_t factor = 3;
        if (e > 1) {
            factor += (seq_t)2 * p * (e - 1);
        }
        result *= factor;
    }
    return result;
}
