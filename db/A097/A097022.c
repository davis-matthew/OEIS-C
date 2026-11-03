/*
 * A097022  a(n) = (sigma(2n^2)-3)/6.
 * https://oeis.org/A097022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (sigma(2 n^2)-3)/6
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 68 terms through n = 879 (2026-11-01)
 * Helpers:   factor_u64, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    /* factor n */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t sigma = 1;
    bool has_two = false;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        uint64_t exp;             /* exponent of p in 2*n^2 */

        if (p == 2) {
            exp = 2 * e + 1;
            has_two = true;
        } else {
            exp = 2 * e;
        }

        seq_t term = ipow_idx((seq_t)p, (idx_t)exp + 1);
        term -= 1;
        term /= (p - 1);          /* integer division, exact */
        sigma *= term;
    }

    if (!has_two) {                 /* 2 was not a factor of n */
        seq_t term = ipow_idx((seq_t)2, (idx_t)2);  /* exp = 1 -> exponent+1 = 2 */
        term -= 1;
        term /= (2 - 1);
        sigma *= term;
    }

    seq_t result = (sigma - (seq_t)3) / 6;
    return result;
}
