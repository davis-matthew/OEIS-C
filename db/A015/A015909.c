/*
 * A015909  a(n) = smallest k >= 1 such that n divides k^n + k.
 * https://oeis.org/A015909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k >= 1 such that n divides k^n + k
 * Range:     u8 n <= 246, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast modular exponentiation using multiplication modulo n.
   All intermediate values are kept within 64‑bit range because
   n <= 10000 in the intended tests. */
static seq_t mod_pow_idx(idx_t base, idx_t exp, idx_t mod)
{
    if (mod == 1) return (seq_t)0;
    idx_t result = 1 % mod;
    idx_t b = base % mod;
    while (exp > 0) {
        if (exp & 1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}

seq_t a(idx_t n)
{
    /* All computations are with positive n; n>=1 by definition. */
    const seq_t sn = (seq_t)n;            /* modulus as seq_t */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute k^n (mod n) */
        seq_t power = mod_pow_idx(k, n, n);
        /* Check if k^n + k is divisible by n */
        if (((power + (seq_t)k) % sn) == (seq_t)0) {
            return (seq_t)k;
        }
    }
    /* Fallback: k = n always satisfies the condition */
    return sn;
}
