/*
 * A138258  Smallest positive integer m such that n divides [5^m/m]
 *          (=A129795(m)).
 * https://oeis.org/A138258
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest positive integer m such that * n divides
 *            floor(5^m / m). (i.e. n | A129795(m))
 * Range:     u8 n <= 21, u16 n <= 1468, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: multiply two values a and b modulo mod.
 * Works for seq_t that fits into uint64_t (all practical n, m).
 * It is safe for 64‑bit seq_t because the intermediate product is
 * computed in unsigned __int128, avoiding overflow. */
static inline seq_t modmul(seq_t a, seq_t b, seq_t mod) {
    uint64_t ua = (uint64_t)a;
    uint64_t ub = (uint64_t)b;
    uint64_t um = (uint64_t)mod;
    unsigned __int128 prod = (unsigned __int128)ua * ub;
    return (seq_t)(prod % um);
}

/* Helper: compute 5^exp mod mod efficiently. */
static inline seq_t pow_mod_5(idx_t exp, seq_t mod) {
    seq_t res = 1;
    seq_t base = 5;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) res = modmul(res, base, mod);
        base = modmul(base, base, mod);
        e >>= 1;
    }
    return res;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    for (idx_t m = 1; ; ++m) {
        seq_t mod = (seq_t)m * n;          /* modulus = m * n */
        seq_t r = pow_mod_5(m, mod);       /* 5^m mod (m*n) */
        if (r < (seq_t)m)                  /* r < m  ⇔  n | floor(5^m/m) */
            return m;                      /* smallest such m */
    }
}
