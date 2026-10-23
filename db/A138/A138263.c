/*
 * A138263  Smallest positive integer m such that n divides [10^m/m]
 *          (=A060156(m)).
 * https://oeis.org/A138263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that n divides
 *            floor(10^m / m)
 * Range:     u8 n <= 11, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to 10^m mod (n*m) == 10^m mod m   */

#include <stdint.h>

static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod) {
    /* use 128-bit intermediate to avoid overflow (mod <= n*m <= 1e8 for n <= 1e4) */
    return (unsigned __int128)a * b % mod;
}

static inline seq_t powmod10(idx_t exp, seq_t mod) {
    seq_t result = 1 % mod;
    seq_t base   = 10 % mod;
    while (exp) {
        if (exp & 1U)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n) {
    /* n >= 1 */
    for (idx_t m = 1; ; ++m) {
        seq_t mod1 = (seq_t)m;
        seq_t mod2 = (seq_t)n * mod1;          /* n*m, fits in 64-bit for n <= 10000 */
        seq_t r1   = powmod10(m, mod1);        /* 10^m mod m */
        seq_t r2   = powmod10(m, mod2);        /* 10^m mod (n*m) */
        if (r2 == r1)          /* then n | floor(10^m/m) */
            return (seq_t)m;
    }
}
