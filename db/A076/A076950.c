/*
 * A076950  Smallest k such that nk-1 is a cube.
 * https://oeis.org/A076950
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k such that n*k - 1 is a perfect cube (i.e. n*k
 *            = m³ + 1 for the least positive integer m)
 * Range:     u8 n <= 19, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n=0, return 0 */
    if (n == 1) return 1;          /* 1*k-1 = m³ → k = m³+1, minimal k=1 (m=0) */

    /* find smallest residue t in [0, n‑1] with t³ ≡ -1 (mod n) */
    for (idx_t t = 0; t < n; ++t) {
        __uint128_t tt = t;
        __uint128_t cube = tt * tt * tt;            /* t³, fits in 128‑bit */
        if ((cube % n) == (n - 1)) {                /* t³ ≡ -1 (mod n) */
            /* k = (t³ + 1) / n, exact division */
            __uint128_t num = cube + 1;
            seq_t k = (seq_t)(num / n);
            return k;
        }
    }
    /* If no residue found (should not happen for n>1), fall back to brute force */
    for (idx_t m = 0; ; ++m) {                     /* search without modulus */
        __uint128_t cube = (__uint128_t)m * m * m;
        if (cube < n) continue;                    /* (m³ + 1)/n not integer yet */
        if (((cube + 1) % n) == 0) {
            seq_t k = (seq_t)((cube + 1) / n);
            return k;
        }
    }
}
