/*
 * A030984  2-automorphic numbers: final digits of 2*n^2 agree with n.
 * https://oeis.org/A030984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the unique integer <10ⁿ such that a ≡ 0 (mod 2ⁿ) and 2a
 *            ≡ 1 (mod 5ⁿ). The function computes a(n) for all n ≥ 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow_seq.h"    /* for pow_seq(seq_t base, int exp) */

seq_t a(idx_t n)
{
    /* 5ⁿ and 2ⁿ */
    seq_t pow5   = pow_seq((seq_t)5, (int)n);      /* 5ᵒⁿ */
    seq_t pow2n  = pow_seq((seq_t)2, (int)n);      /* 2ᵒⁿ */

    /* inv2 = (5ⁿ + 1) / 2  ≡ inverse of 2 modulo 5ⁿ   */
    seq_t inv2   = (pow5 + (seq_t)1) >> 1;         /* (5ⁿ+1)/2 */

    /* Calculate inverse of 5ⁿ modulo 2ⁿ by successive lifting.
       Start with the inverse modulo 2 (which is 1). */
    seq_t inv5n  = (seq_t)1;
    seq_t pow2k  = (seq_t)2;                       /* current modulus 2ᵏ */

    for (idx_t k = 1; k <= n; ++k) {
        if (k > 1)
            pow2k *= (seq_t)2;                      /* pow2k = 2ᵏ */
        /* 5ⁿ modulo current modulus */
        seq_t a_mod = pow5 % pow2k;
        seq_t temp   = (a_mod * inv5n) % pow2k;
        seq_t diff   = (pow2k + (seq_t)2 - temp) % pow2k;
        inv5n       = (inv5n * diff) % pow2k;
    }

    /* t ≡ –inv2 * inverse(5ⁿ) (mod 2ⁿ) */
    seq_t inv2_mod = inv2 % pow2n;                 /* keep 0 ≤ inv2_mod < 2ⁿ */
    seq_t t  = ((pow2n - inv2_mod) * inv5n) % pow2n;

    /* The unique solution: a = inv2 + 5ⁿ * t  (≡ 0 mod 2ⁿ, ≡ inv2 mod 5ⁿ). */
    return inv2 + pow5 * t;
}
