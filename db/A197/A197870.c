/*
 * A197870  Expansion of false theta series variation of Ramanujan theta
 *          function psi(x).
 * https://oeis.org/A197870
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^k if n = k*(k+1)/2 else 0
 * Range:     i8 n <= 7, i16 n <= 1681, i32 n <= 5000, i64 n <= 5000,
 *            i128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;                       /* n as seq_t */
    seq_t disc = (seq_t)1 + (seq_t)8 * tn;      /* 1 + 8n   */
    seq_t sq = floor_sqrt(disc);               /* ⌊√disc⌋   */
    if (sq * sq != disc)                       /* not a perfect square → not triangular */
        return (seq_t)0;
    seq_t k = (sq - (seq_t)1) / (seq_t)2;       /* k = (-1 + √disc)/2  */
    /* double‑check that k satisfies the equation exactly */
    if ((k * (k + (seq_t)1)) / (seq_t)2 != tn)
        return (seq_t)0;
    /* sign is (-1)^k */
    return (k & (seq_t)1) ? (seq_t)-1 : (seq_t)1;
}
