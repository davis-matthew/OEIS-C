/*
 * A067007  a(n) = googolplex (mod n).
 * https://oeis.org/A067007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^(10^100) (mod n) (googolplex modulo n)
 * Range:     u8 n <= 264, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Small modular multiplication that works for 64‑bit seq_t.
 * For larger seq_t types the builtin multiplication of the bigint class
 * will be used automatically. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod) {
#if defined(__UINT128_TYPE__) || defined(__cplusplus)
    return (seq_t)(((__uint128_t)a * b) % mod);
#else
    return (a * b) % mod;
#endif
}

/* fast exponentiation for a small exponent (<=10 here). */
static inline seq_t pow_small(seq_t base, int exp, seq_t mod) {
    seq_t result = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1)
            result = mul_mod(result, base, mod);
        base   = mul_mod(base, base, mod);
        exp >>= 1;
    }
    return result;
}

/* a(n) = googolplex (mod n).  n >= 1. */
seq_t a(idx_t n) {
    if (n == 0)
        return 0;          /* out of range, but harmless */
    seq_t mod = (seq_t)n;
    if (mod == 1)
        return 0;          /* everything is 0 mod 1 */

    /* Base 10 modulo n */
    seq_t base = 10 % mod;

    /* Pre‑compute a^d for d = 0..9 */
    seq_t small[10];
    small[0] = 1 % mod;
    for (int d = 1; d < 10; ++d)
        small[d] = mul_mod(small[d-1], base, mod);

    seq_t result = 1 % mod;          /* result = a^0 initially */
    const int digits = 101;          /* 10^100 has 101 decimal digits (1 followed by 100 zeros) */

    for (int i = 0; i < digits; ++i) {
        int d = (i == 0 ? 1 : 0);     /* first digit is 1, the rest are 0 */

        /* result = result^10 mod mod */
        seq_t pow10 = 1 % mod;
        seq_t b = result % mod;
        int e = 10;
        while (e) {
            if (e & 1)
                pow10 = mul_mod(pow10, b, mod);
            b = mul_mod(b, b, mod);
            e >>= 1;
        }

        /* result = pow10 * a^d mod mod */
        result = mul_mod(pow10, small[d], mod);
    }
    return result;
}
