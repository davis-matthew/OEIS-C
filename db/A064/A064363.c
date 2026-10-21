/*
 * A064363  Number of 2 X 2 regular integer matrices with elements from
 *          {0,...,n} up to row and column permutation.
 * https://oeis.org/A064363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A064363
   Formula: a(n) = ((n+1)*(n^3 + 3*n^2 + 4*n + 1) - A059306(n)) / 4
   where A059306(n) = (n+1)^2 + Σ_{m=1}^n C(m)·⌊n/m⌋·(⌊n/m⌋+1),
   with C(1)=3 and for m>1: C(m)=2·ϕ(m) (Euler's totient) */
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Trivial value for n = 0 */
    if (n == 0) return 0;

    /* ---------- Compute Euler totient values up to n ---------- */
    uint64_t *phi = (uint64_t *)malloc((n + 1) * sizeof(uint64_t));
    if (!phi) return 0; /* allocation failure – return 0 (should not occur in tests) */

    for (idx_t i = 0; i <= n; ++i) phi[i] = i;
    for (idx_t p = 2; p <= n; ++p) {
        if (phi[p] == p) { /* p is prime */
            for (idx_t j = p; j <= n; j += p) {
                phi[j] -= phi[j] / p;
            }
        }
    }

    /* ---------- Compute A059306(n) ---------- */
    seq_t A059306 = (seq_t)(n + 1) * (seq_t)(n + 1); /* (n+1)^2 */
    for (idx_t m = 1; m <= n; ++m) {
        seq_t Cm = (m == 1 ? (seq_t)3 : (seq_t)(2 * phi[m]));
        seq_t t  = (seq_t)(n / m);             /* floor(n/m) */
        A059306 += Cm * t * (t + (seq_t)1);
    }

    free(phi);

    /* ---------- Compute the main numerator B ---------- */
    seq_t sn = (seq_t)n;
    seq_t B  = (sn + (seq_t)1) *
              (sn * sn * sn + (seq_t)3 * sn * sn + (seq_t)4 * sn + (seq_t)1);

    /* ---------- Final result ---------- */
    return (B - A059306) / (seq_t)4;
}
