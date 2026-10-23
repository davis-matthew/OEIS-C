/*
 * A136700  Final nonzero digit of n! in base 14.
 * https://oeis.org/A136700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the final non‑zero digit of n! in base 14
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* fast modular power for small bases */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod) {
    uint64_t result = 1;
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Special case n == 0 */
    if (n == 0) return 1;

    /* Count powers of 2 and 7 in n! using Legendre's formula */
    uint64_t v2 = 0, v7 = 0;
    idx_t t = n;
    while (t) { t /= 2; v2 += t; }
    t = n;
    while (t) { t /= 7; v7 += t; }

    uint64_t k  = (v2 < v7) ? v2 : v7;     /* number of 14⁻factors removed */
    uint64_t e2 = v2 - k;                  /* remaining 2‑exponent */
    uint64_t e7 = v7 - k;                  /* remaining 7‑exponent */

    /* Compute the product of all factors after stripping 2’s and 7’s */
    seq_t prod = 1;
    for (idx_t i = 1; i <= n; ++i) {
        idx_t x = i;
        while ((x & 1) == 0) x >>= 1;     /* remove all factors of 2 */
        while (x % 7 == 0) x /= 7;         /* remove all factors of 7 */
        prod = (prod * (seq_t)(x % 14)) % 14;
    }

    /* Multiply back the leftover powers of 2 and 7 */
    prod = (prod * (seq_t)(mod_pow_uint64(2, e2, 14))) % 14;
    prod = (prod * (seq_t)(mod_pow_uint64(7, e7, 14))) % 14;

    return prod;
}
