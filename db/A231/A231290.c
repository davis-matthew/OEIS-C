/*
 * A231290  Numbers k dividing u(k), where the Lucas sequence is defined u(i)
 *          = u(i-1) - 5*u(i-2) with initial conditions u(0)=0, u(1)=1.
 * https://oeis.org/A231290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
 * Helpers:   mulmod_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A231290
    a(n) is the n‑th smallest positive integer k such that k divides
    the Lucas sequence term u_k defined by
        u_0 = 0,   u_1 = 1,
        u_i = u_{i-1} - 5 * u_{i-2}.
    For n = 0 the first term is 1.
*/

#include <stdint.h>
#include "../utils/mulmod_idx.h"

/*  Computes u_k modulo k  (k > 0).  Returns a value in [0, k‑1]. */
static idx_t u_mod_idx(idx_t k)
{
    idx_t mod = k;

    /* matrix M = [[1, -5], [1, 0]]  (entries taken modulo mod) */
    idx_t m11 = 1 % mod;
    idx_t m12 = (mod + mod - 5) % mod;   /* -5 mod mod */
    idx_t m21 = 1 % mod;
    idx_t m22 = 0 % mod;

    /* result matrix starts as identity */
    idx_t r11 = 1 % mod;
    idx_t r12 = 0;
    idx_t r21 = 0;
    idx_t r22 = 1 % mod;

    idx_t exp = k - 1;            /* exponent of M */

    while (exp) {
        if (exp & 1) {
            /* r = r * M  (mod mod) */
            idx_t t11 = (mulmod_idx(r11, m11, mod) + mulmod_idx(r12, m21, mod)) % mod;
            idx_t t12 = (mulmod_idx(r11, m12, mod) + mulmod_idx(r12, m22, mod)) % mod;
            idx_t t21 = (mulmod_idx(r21, m11, mod) + mulmod_idx(r22, m21, mod)) % mod;
            idx_t t22 = (mulmod_idx(r21, m12, mod) + mulmod_idx(r22, m22, mod)) % mod;
            r11 = t11; r12 = t12; r21 = t21; r22 = t22;
        }
        /* M = M * M (mod mod) */
        idx_t t11 = (mulmod_idx(m11, m11, mod) + mulmod_idx(m12, m21, mod)) % mod;
        idx_t t12 = (mulmod_idx(m11, m12, mod) + mulmod_idx(m12, m22, mod)) % mod;
        idx_t t21 = (mulmod_idx(m21, m11, mod) + mulmod_idx(m22, m21, mod)) % mod;
        idx_t t22 = (mulmod_idx(m21, m12, mod) + mulmod_idx(m22, m22, mod)) % mod;
        m11 = t11; m12 = t12; m21 = t21; m22 = t22;

        exp >>= 1;
    }

    /* u_k is the first component of r * [1,0]ᵀ  -> r11  */
    return r11 % mod;
}

/*  The required function.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;      /* a(0) = 1 */

    idx_t count = 0;
    /* the first term (k = 1) is already handled, start checking from k = 2 */
    for (idx_t k = 2; ; ++k) {
        if (u_mod_idx(k) == 0) {      /* k divides u_k */
            ++count;
            if (count == n)
                return (seq_t)k;
        }
    }
}
