/*
 * A175068  a(n) = product of perfect divisors of n.
 * https://oeis.org/A175068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product of all perfect divisors of n A perfect divisor d
 *            of n satisfies d^k = n for some integer k ≥ 1
 * Range:     u8 n <= 48, u16 n <= 624, u32 n <= 9898, u64 n <= 9898,
 *            u128 n <= 9898, big any n
 * Helpers:   euclidean_gcd, factor_u64, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/pow_uint.h"
#include "../utils/euclidean_gcd.h"

seq_t a(idx_t n) {
    if (n == 1) return 1;          /* the only perfect divisor of 1 is 1 */
    
    /* Factor n (up to 64‑bit). */
    uint64_t primes[16];
    idx_t    exps[16];
    size_t   cnt = 0;
    factor_u64(n, primes, exps, &cnt);
    
    /* Compute G = gcd of all exponents. */
    idx_t G = exps[0];
    for (size_t i = 1; i < cnt; ++i)
        G = euclidean_gcd(G, exps[i]);
    
    /* Enumerate all divisors of G (G ≤ 64 for 64‑bit n). */
    idx_t divs[64];
    size_t dcnt = 0;
    for (idx_t d = 1; d <= G; ++d)
        if (G % d == 0) divs[dcnt++] = d;
    
    /* Multiply the corresponding roots. */
    seq_t result = 1;
    for (size_t j = 0; j < dcnt; ++j) {
        idx_t d = divs[j];
        seq_t root = 1;
        for (size_t i = 0; i < cnt; ++i) {
            idx_t exp = exps[i] / d;
            root *= pow_uint(primes[i], exp);
        }
        result *= root;
    }
    return result;
}
