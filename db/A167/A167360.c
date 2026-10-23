/*
 * A167360  Totally multiplicative sequence with a(p) = (p+2)*(p+3) = p^2+5p+6
 *          for prime p.
 * https://oeis.org/A167360
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} ((p+2)*(p+3))^e
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/factor_u64.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* maximum number of distinct prime factors of a 64‑bit integer is 64 */
    uint64_t pr[64];
    uint64_t ex[64];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        seq_t base = ((seq_t)p + (seq_t)2) * ((seq_t)p + (seq_t)3);
        seq_t pow  = seq_pow(base, (idx_t)ex[i]);
        result = seq_mul(result, pow);
    }

    return result;
}
