/*
 * A227477  Exponent of the group of Lipschitz quaternions in a reduced system
 *          modulo n.
 * https://oeis.org/A227477
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = lcm_over_prime_powers[ p^e if p==2 else p^e * (p^2-1) ]
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Validated: partial, pass, 10 terms through n = 10 (2026-11-01)
 * Helpers:   gcd, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"
#include "../utils/ipow_u64_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

static inline seq_t lcm_seq(seq_t a, seq_t b)
{
    seq_t g = gcd(a, b);
    seq_t res = seq_mul(a / g, b);
    return res;
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t k = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* handle factor 2 */
    int e = 0;
    while (k % 2 == 0) {
        k /= 2;
        ++e;
    }
    if (e > 0) {
        seq_t val = ipow_u64_idx(2, e);
        result = lcm_seq(result, val);
    }

    /* handle odd prime factors */
    for (uint64_t p = 3; p * p <= k; p += 2) {
        if (k % p == 0) {
            int exp = 0;
            while (k % p == 0) {
                k /= p;
                ++exp;
            }
            seq_t powpe = ipow_u64_idx(p, exp);
            seq_t factor = (seq_t)(p * p - 1);
            seq_t val = seq_mul(powpe, factor);
            result = lcm_seq(result, val);
        }
    }

    /* remaining prime factor, if any */
    if (k > 1) {
        seq_t powpe = (seq_t)k;   /* exponent is 1 */
        seq_t factor = (seq_t)(k * k - 1);
        seq_t val = seq_mul(powpe, factor);
        result = lcm_seq(result, val);
    }

    return result;
}
