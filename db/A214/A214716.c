/*
 * A214716  Least m>0 such that 3^n-m and n-m are relatively prime.
 * https://oeis.org/A214716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 589, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) as the smallest m > 0 with gcd(3^n - m, n - m) = 1
   by iteratively testing m and using modular reduction to avoid
   constructing the huge number 3^n. */
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Special handling for the trivial case n = 1 */
    if (n == (idx_t)1) {
        return (seq_t)2;
    }

    idx_t limit = n + (idx_t)1;          /* m ≤ n-1 guaranteed for n≥2, so n+1 covers all */
    for (idx_t m = (idx_t)1; m <= limit; ++m) {
        /* Compute |n - m| as a seq_t */
        seq_t d = (seq_t)((n >= m) ? (n - m) : (m - n));
        if (d == 0) {
            continue;                    /* m == n yields n - m = 0; skip */
        }
        if (d == (seq_t)1) {
            return (seq_t)m;              /* gcd always 1 when modulus is 1 */
        }

        /* Compute 3^n mod d */
        seq_t r = mod_pow((seq_t)3, n, d);

        /* Compute (3^n - m) mod d */
        seq_t m_mod = ((seq_t)m) % d;
        seq_t diff  = (r + d - m_mod) % d;  /* non-negative remainder */

        /* Check coprimeness */
        if (gcd(diff, d) == (seq_t)1) {
            return (seq_t)m;
        }
    }

    /* Fallback (should not be reached for n ≥ 1) */
    return (seq_t)0;
}
