/*
 * A094924  a(n) = (9^n-1)/8 mod n.
 * https://oeis.org/A094924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (9^n-1)/8 mod n
 * Range:     u8 n <= 31, u16 n <= 8783, u32 n <= 9955, u64 n <= 9955,
 *            u128 n <= 9955, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Multiply two seq_t numbers modulo mod. Uses 128‑bit for safety. */
static inline seq_t mul_mod(seq_t a, seq_t b, seq_t mod)
{
    return (seq_t)((unsigned __int128)a * b % mod);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset starts at 1, but guard anyway */

    seq_t mod = (seq_t)8 * (seq_t)n;      /* 8*n fits comfortably in seq_t for n up to 10000 */
    seq_t base = 9 % mod;
    seq_t result = 1 % mod;
    idx_t exp = n;

    /* binary modular exponentiation */
    while (exp) {
        if (exp & 1) result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1;
    }

    /* result now holds 9^n mod (8*n), which is of the form 1 + 8*t, 0 <= t < n */
    seq_t r = result;
    seq_t t = (r - 1) / 8;   /* integer division, ensures t < n */
    return t;                /* t is already the value modulo n */
}
