/*
 * A060692  Number of parts if 3^n is partitioned into parts of size 2^n as
 *          far as possible and into parts of size 1^n.
 * https://oeis.org/A060692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3^n / 2^n) + (3^n mod 2^n) for n >= 1 Equivalent
 *            for n > 2: a(n) = 3^n mod (2^n – 1)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 32, u64 n <= 65, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Utility for the power of 2 used by the big‑int path */
#include <stdint.h>
#include "../utils/pow2.h"

 /* Compute 3^exp modulo mod for arbitrary‑precision seq_t */
static seq_t mod_pow_seq(seq_t base, idx_t exp, seq_t mod)
{
    seq_t result = 1 % mod;        /* keeps the same type */
    base %= mod;
    while (exp) {
        if (exp & 1) result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/* Main sequence generator */
seq_t a(idx_t n)
{
    /* Offset: a(1)=2, a(2)=3 */
    if (n == 1) return 2;
    if (n == 2) return 3;

    /* Small (≤64‑bit) seq_t: use 128‑bit intermediates */
    if (sizeof(seq_t) <= 8) {
        __uint128_t p = 1;        /* 3^n */
        __uint128_t two_n = 1;    /* 2^n */
        for (idx_t i = 0; i < n; ++i) {
            p *= 3;
            two_n *= 2;
        }
        __uint128_t val = p / two_n + p % two_n; /* floor + remainder */
        return (seq_t)val;      /* implicit truncation is fine for n
                                   where the final value fits in seq_t */
    }

    /* Arbitrary‑precision seq_t: compute a(n) = 3^n mod (2^n‑1) */
    seq_t modulus = pow2(n);   /* 2^n */
    modulus -= 1;              /* 2^n – 1 */
    return mod_pow_seq((seq_t)3, n, modulus);
}
