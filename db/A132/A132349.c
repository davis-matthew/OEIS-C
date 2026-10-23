/*
 * A132349  If n is a k-th power with k >= 2 then a(n) = k, otherwise a(n) =
 *          0.
 * https://oeis.org/A132349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k if n is a k-th power with k >= 2, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, u64_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/u64_gcd.h"

seq_t a(idx_t n) {
    if (n <= 1) return 0;
    uint64_t num = (uint64_t)n;

    /* arrays large enough for all 64‑bit factorizations */
    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt;

    factor_u64(num, primes, exps, &cnt);
    if (cnt == 0) return 0;  /* safety, should not occur for num > 1 */

    uint64_t k = exps[0];
    for (size_t i = 1; i < cnt; ++i) {
        k = u64_gcd(k, exps[i]);
    }

    return (k >= 2) ? (seq_t)k : 0;
}
