/*
 * A131135  Denominator of (ordinary) expansion of log((x/2-1)/(x-1)).
 * https://oeis.org/A131135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*2^n / gcd(n, 2^n-1) for n>0, a(0)=1
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Helpers:   gcd_uint64, mul_mod64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/mul_mod64.h"
#include "../utils/seq_pow.h"
#include "../utils/gcd_uint64.h"

static uint64_t mod_pow_2_uint64(uint64_t exp, uint64_t mod) {
    if (mod == 1) return 0;
    uint64_t base = 2 % mod;
    uint64_t result = 1 % mod;
    while (exp) {
        if (exp & 1ULL) result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    uint64_t u = (uint64_t)n;
    uint64_t mod = mod_pow_2_uint64(u, u);
    uint64_t diff = (mod + u - 1) % u;            /* (2^n - 1) mod n */
    uint64_t d = gcd_uint64(u, diff);             /* gcd(n, 2^n-1) */

    seq_t pow2 = seq_pow((seq_t)2, n);             /* 2^n */
    seq_t numerator = (seq_t)n * pow2;            /* n * 2^n */
    seq_t result = numerator / (seq_t)d;          /* division exact */

    return result;
}
