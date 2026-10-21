/*
 * A209991  Number of 2 X 2 matrices with all elements in {0,1,...,n} and
 *          determinant in {0,1}.
 * https://oeis.org/A209991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of 2×2 matrices with elements in {0,…,n} and determinant
 *            in {0,1} * This implementation computes the value a(n) for any n
 *            ≥ 0
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   egcd_int, v_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/egcd_int.h"
#include "../utils/v_gcd.h"

seq_t a(idx_t n)
{
    /* ---------- Determinant 0 part ---------- */
    seq_t n1 = (seq_t)(n + 1);                /* (n+1) */
    seq_t S0 = n1 * n1;                       /* a = b = 0 case */
    S0 += (seq_t)2 * n * n1;                 /* a = 0, b>0 or a>0, b=0 */

    /* Compute Euler's totient values up to n. */
    if (n >= 1) {
        int *phi = (int*)malloc((n + 1) * sizeof(int));
        if (phi) {
            for (idx_t i = 0; i <= n; ++i) phi[i] = (int)i;
            for (idx_t i = 2; i <= n; ++i) {
                if (phi[i] == (int)i) {           /* i is prime */
                    for (idx_t j = i; j <= n; j += i)
                        phi[j] -= phi[j] / (int)i;
                }
            }
            /* Add contributions for all pairs (a,b) with a>0,b>0. */
            for (idx_t L = 1; L <= n; ++L) {
                seq_t G = (seq_t)(n / L);
                if (G == 0) continue;
                seq_t countC = (seq_t)(2 * phi[L] - (L == 1 ? 1 : 0));
                S0 += countC * G * (G + 1);
            }
            free(phi);
        }
    }

    /* --------- Determinant 1 part --------- */
    seq_t S1 = 0;

    /* a = 1, b = 0 case (if 1 ≤ n) */
    if (n >= 1) S1 += (seq_t)(n + 1);

    /* a = 1, b > 0 case */
    if (n >= 1) {
        for (idx_t b = 1; b <= n; ++b) {
            /* Count c such that 1 + b*c ≤ n */
            seq_t cnt = ((seq_t)(n - 1) / (seq_t)b) + 1;
            S1 += cnt;
        }
    }

    /* a > 1, b > 0, gcd(a,b) = 1 */
    for (idx_t a = 2; a <= n; ++a) {
        for (idx_t b = 1; b <= n; ++b) {
            if (v_gcd(a, b) != 1) continue;

            /* Compute modular inverse of b modulo a (1 ≤ a ≤ 10^4). */
            int inv_b;
            {
                int x, y;
                egcd_int((int)a, (int)b, &x, &y);  /* y is inverse of b mod a */
                inv_b = y % (int)a;
                if (inv_b < 0) inv_b += (int)a;
            }

            /* Minimal positive solution for the linear Diophantine system */
            int64_t c0 = (int64_t)a - inv_b;                    /* 1 ≤ c0 ≤ a-1 */
            int64_t d0 = (int64_t)(1 + b * c0) / (int64_t)a;    /* 1 ≤ d0 ≤ b‑1 */

            /* Determine permissible t */
            int64_t tmax_a = ((int64_t)n - c0) / (int64_t)a;
            int64_t tmax_b = ((int64_t)n - d0) / (int64_t)b;
            int64_t tmax   = tmax_a < tmax_b ? tmax_a : tmax_b;

            if (tmax >= 0) S1 += (seq_t)(tmax + 1);
        }
    }

    return S0 + S1;
}
