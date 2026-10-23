/*
 * A134464  (A127648 * A000012 + A000012 * A127773) - A000012.
 * https://oeis.org/A134464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A134464 – triangle read by rows:
 *  a(r,k) = (r-1) + k*(k+1)/2   (1 ≤ k ≤ r)
 *  with r = smallest integer such that r*(r+1)/2 ≥ n
 *  and k = n − (r‑1)*r/2
 */

#include "../utils/floor_sqrt.h"   /* floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    if (n == 0)           /* offset is 1 – guard against n==0 */
        return 0;

    /* ----- find row r :  r(r+1)/2 ≥ n  ----- */
    /* use integer sqrt of 8*n+1:  r ≈ (sqrt(8n+1)-1)/2 */
    seq_t t = (seq_t)8 * (seq_t)n + 1;            /* 8*n+1   */
    seq_t s = floor_sqrt(t);                     /* floor sqrt(8n+1) */
    seq_t r_seq = (s - 1) / 2;                   /* floor((sqrt-1)/2) */
    idx_t r = (idx_t)r_seq;                      /* r fits in idx_t for our range */

    /* correct r if necessary (rare, when n is a triangular number) */
    seq_t triR = (seq_t)r * (r + 1) / 2;
    if (triR < (seq_t)n)
    {
        r++;
        triR = (seq_t)r * (r + 1) / 2;           /* recompute once more */
    }

    /* ----- compute k = n - (r-1)*r/2 ----- */
    seq_t triRm1 = (seq_t)(r - 1) * r / 2;       /* T(r-1) */
    seq_t k_seq = (seq_t)n - triRm1;             /* always fits in seq_t */
    idx_t k = (idx_t)k_seq;                      /* for clarity */

    /* ----- final value a(r,k) ----- */
    seq_t val = (seq_t)(r - 1) + k_seq * (k_seq + 1) / 2;
    return val;
}
