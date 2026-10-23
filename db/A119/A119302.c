/*
 * A119302  Inverse of number triangle binomial(3n-k,n-k).
 * https://oeis.org/A119302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = (C(2k,r‑k)+3·C(2k,r‑k‑1))·(–1)^(r‑k) where r =
 *            ⌊(√(8n+1)–1)/2⌋ and k = n – r(r+1)/2
 * Range:     i8 n <= 31, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is the flattened triangle T with
   T(r,k) = (C(2k,r‑k)+3*C(2k,r‑k‑1))·(–1)^(r‑k) for 0 ≤ k ≤ r. */

#include <stdint.h>
#include "../utils/binomial.h"

/* Integer square‑root: floor(sqrt(x)) for unsigned 64‑bit x. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t r = 0;
    for (int bit = 62; bit >= 0; --bit) {
        idx_t t = r | ((idx_t)1 << bit);
        if (t <= x / t)  /* avoid overflow: t*t ≤ x */
            r = t;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* row r and column k in the flattened triangle */
    idx_t r      = (isqrt_u64(8 * n + 1) - 1) >> 1;      /* floor((√(8n+1)–1)/2) */
    idx_t start  = r * (r + 1) / 2;                      /* entries before row r */
    idx_t k      = n - start;                            /* column index in row r */

    idx_t d = r - k;        /* n – k */

    seq_t term1 = 0;
    seq_t term2 = 0;

    if (d <= 2 * k)
        term1 = binomial(2 * k, d);          /* C(2k, n‑k)   */

    if (d > 0 && d - 1 <= 2 * k)
        term2 = binomial(2 * k, d - 1);      /* C(2k, n‑k‑1) */

    seq_t sum  = term1 + 3 * term2;
    seq_t sign = (d & 1) ? (seq_t)(-1) : (seq_t)1;   /* (–1)^(n‑k) */
    return sum * sign;
}
