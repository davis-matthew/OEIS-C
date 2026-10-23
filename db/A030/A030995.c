/*
 * A030995  9-automorphic numbers ending in 5: final digits of 9*n^2 agree
 *          with n.
 * https://oeis.org/A030995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 5^n * inv_mod(9*5^n, 2^n) (mod 2^n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   big_pow, inv_mod_odd_power_of_two, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the unique integer <10^n that is divisible by 5^n and
   satisfies 9*a(n) ≡ 1 (mod 2^n).  The following implementation
   computes it directly using the Chinese Remainder Theorem. */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/pow2.h"
#include "../utils/inv_mod_odd_power_of_two.h"

/* OEIS sequence A030995: 9‑automorphic numbers ending in 5. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0, but safe return */

    seq_t pow2_n   = pow2(n);           /* 2^n */
    seq_t pow5_n   = big_pow((seq_t)5, (seq_t)n); /* 5^n */
    seq_t pow5_mod2 = pow5_n % pow2_n;           /* 5^n mod 2^n */

    seq_t inv5    = inv_mod_odd_power_of_two(pow5_mod2, n); /* inverse of 5^n mod 2^n */
    seq_t inv9    = inv_mod_odd_power_of_two((seq_t)9, n);   /* inverse of 9 mod 2^n */

    seq_t t     = (inv9 * inv5) % pow2_n;   /* k = 9^{-1} * (5^n)^{-1} mod 2^n */
    seq_t res   = pow5_n * t;               /* a(n) = 5^n * k */

    return res;
}
