/*
 * A093846  Triangle read by rows: T(n, k) = 10^(n-1) - 1 +
 *          k*floor(9*10^(n-1)/n), for 1 <= k <= n.
 * https://oeis.org/A093846
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, less, nonn, tabl
 * Formula:   a(n) = 10^(r-1)-1 + k*floor(9*10^(r-1)/r), where r = smallest
 *            integer with r(r+1)/2 ≥ n and k = n - (r-1)r/2 (1‑based position
 *            in row r)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 48, u64 n <= 191, u128 n <= 751,
 *            big any n
 * Helpers:   floor_sqrt, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"          /* seq_t pow10(idx_t e);                */
#include "../utils/floor_sqrt.h"    /* seq_t floor_sqrt(seq_t x);           */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset 1 – undefined for n=0 */

    /* Find the row r that contains the n‑th element. */
    seq_t sq   = floor_sqrt((seq_t)1 + (seq_t)8 * (seq_t)n);  /* floor(sqrt(1+8n)) */
    idx_t r    = (idx_t)((sq - (seq_t)1) / (seq_t)2);         /* floor((sqrt-1)/2) */
    /* Ensure r is minimal: if (r-1)*r/2 < n increase r. */
    seq_t tri  = (seq_t)r * (r + 1) / 2;
    if (tri < n) r++;

    /* Position k within that row (1‑based). */
    idx_t prev = (idx_t)((seq_t)(r - 1) * r / 2);
    idx_t k    = n - prev;

    /* Compute base and step of the arithmetic progression. */
    seq_t pow   = pow10(r - 1);          /* 10^(r-1) */
    seq_t base  = pow - (seq_t)1;        /* 10^(r-1)-1 */

    /* diff = floor(9*pow / r) – avoid overflow by dividing first. */
    seq_t q     = pow / r;
    seq_t rem   = pow % r;
    seq_t diff  = q * (seq_t)9 + (rem * (seq_t)9) / r;

    /* Final term: base + k*diff. */
    return base + (seq_t)k * diff;
}
