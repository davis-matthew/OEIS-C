/*
 * A030991  7-automorphic numbers ending in 5: final digits of 7n^2 agree with
 *          n.
 * https://oeis.org/A030991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow2, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the unique integer <10^n such that
   a(n) ≡ 0   (mod 5^n)   and   7*a(n) ≡ 1 (mod 2^n)        */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"   /* pow_seq(base, exp)                     */
#include "../utils/pow2.h"      /* pow2(n)   returns 2^n                  */

/* Compute the inverse of an odd integer a modulo 2^k.
   The second argument is the modulus 2^k.            */
static seq_t inv_mod_pow2(seq_t a, seq_t mod)
{
    seq_t inv = 1;          /* inverse modulo 2 */
    seq_t cur_mod = 2;      /* current modulus 2^i */

    /* iterate until cur_mod == mod  (i.e. we have 2^k) */
    while (cur_mod < mod) {
        seq_t new_mod = cur_mod << 1;          /* 2^(i+1) */
        seq_t mult = (a * inv) % new_mod;      /* a*inv  (mod new_mod) */
        seq_t temp = (new_mod + (seq_t)2 - mult) % new_mod;
        inv = (inv * temp) % new_mod;          /* new inverse modulo new_mod */
        cur_mod = new_mod;
    }
    return inv;            /* already < mod */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset 1 – optional guard */

    /* 5^n and 2^n */
    seq_t pow5 = pow_seq((seq_t)5, (int)n);   /* 5^n  */
    seq_t pow2_val = pow2(n);                /* 2^n  */

    /* t = 5^n mod 2^n  (odd, hence invertible modulo 2^n) */
    seq_t t = pow5 % pow2_val;

    /* Inverses modulo 2^n */
    seq_t inv7 = inv_mod_pow2((seq_t)7, pow2_val);
    seq_t invt = inv_mod_pow2(t, pow2_val);

    /* k = inv7 * invt  (mod 2^n) */
    seq_t k = (inv7 * invt) % pow2_val;

    /* a(n) = k * 5^n  (< 10^n automatically) */
    return k * pow5;
}
