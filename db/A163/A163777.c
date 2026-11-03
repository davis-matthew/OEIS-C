/*
 * A163777  Even terms in the sequence of Queneau numbers A054639.
 * https://oeis.org/A163777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = 2*A137310(n) (even numbers N such that p=2N+1 prime, p ≡5
 *            (mod8) and 2 is a primitive root modulo p)
 * Range:     u8 n <= 15, u16 n <= 1263, u32 n <= 3378, u64 n <= 3378,
 *            u128 n <= 3378, big any n
 * Validated: standard, pass, 71 terms through n = 3282 (2026-11-01)
 * Helpers:   factor_u64, is_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/powmod.h"

static bool has_primitive_root_2(uint64_t p) {
    /* p is an odd prime */
    uint64_t phi = p - 1;
    /* Distinct prime factors of phi; 32 slots is ample for any 64‑bit n */
    uint64_t pr[32];
    uint64_t ex[32];
    size_t cnt = 0;
    factor_u64(phi, pr, ex, &cnt);
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t q = pr[i];
        seq_t res = powmod((seq_t)2, (idx_t)(phi / q), (seq_t)p);
        if (res == (seq_t)1)
            return false;
    }
    return true;
}

seq_t a(idx_t n) {
    idx_t count = 0;
    for (uint64_t p = 5; ; ++p) {          /* start with 5 (prime, 5 mod 8) */
        if (p % 8 != 5)
            continue;
        if (!is_prime(p))
            continue;
        if (!has_primitive_root_2(p))
            continue;
        ++count;
        if (count == n)
            return (seq_t)((p - 1) / 2);
    }
    /* unreachable */
    return 0;
}
