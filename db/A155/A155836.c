/*
 * A155836  2^(2^n) mod n.
 * https://oeis.org/A155836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^(2^n) mod n
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply in a way that works for seq_t up to 64‑bit.  If the
   compiler provides 128‑bit integers, use them; otherwise fall back
   to multiplication in an unsigned long long, which is at least 64‑bit. */
static inline seq_t mul_mod_seq(seq_t a, seq_t b, seq_t mod) {
#if defined(__SIZEOF_INT128__)
    return (seq_t)(((__uint128_t)a * b) % mod);
#else
    unsigned long long prod = (unsigned long long)a * (unsigned long long)b;
    return (seq_t)(prod % (unsigned long long)mod);
#endif
}

/* Compute A155836: 2^(2^n) mod n */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* defensive guard (not used) */
    seq_t mod = (seq_t)n;
    seq_t r = 2 % mod;               /* start with 2^1 mod n */

    /* One squaring per exponentiation step.  After n squarings we get
       2^(2^n) mod n. */
    for (idx_t i = 0; i < n; ++i) {
        r = mul_mod_seq(r, r, mod);
    }
    return r;
}
