/*
 * A166642  Totally multiplicative sequence with a(p) = 2*(p+1) for prime p.
 * https://oeis.org/A166642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = ∏_{p^e‖n} (2·(p+1))^e Totally multiplicative sequence
 *            with a(p) = 2*(p+1) for prime p
 * Range:     u8 n <= 11, u16 n <= 127, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9560 (2026-11-01)
 * Helpers:   factor_u64, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = (seq_t)1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        seq_t base   = (seq_t)2 * ((seq_t)p + (seq_t)1);
        seq_t pwr    = ipow(base, (seq_t)exps[i]);
        result *= pwr;
    }

    return result;
}
