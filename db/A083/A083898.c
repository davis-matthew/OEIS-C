/*
 * A083898  Number of divisors of n with largest digit <= 3 (base 10).
 * https://oeis.org/A083898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Number of divisors of n with all decimal digits ≤ 3
 * Range:     u8 n <= 286, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digits_le3, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/digits_le3.h"

/* recursively enumerate divisors and count those with digits ≤ 3 */
static seq_t rec_div(const uint64_t *pr, const uint64_t *ex,
                     size_t pcnt, size_t pos, seq_t cur) {
    if (pos == pcnt) {
        return digits_le3(cur) ? (seq_t)1 : (seq_t)0;
    }
    seq_t total = 0;
    seq_t val = 1;
    for (size_t e = 0; e <= ex[pos]; ++e) {
        total += rec_div(pr, ex, pcnt, pos + 1, cur * val);
        val *= (seq_t)pr[pos];
    }
    return total;
}

seq_t a(idx_t n) {
    uint64_t nn = (uint64_t)n;
    if (nn == 0) return (seq_t)0;           /* undefined but guard */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t pcnt = 0;
    factor_u64(nn, primes, exps, &pcnt);
    return rec_div(primes, exps, pcnt, 0, (seq_t)1);
}
