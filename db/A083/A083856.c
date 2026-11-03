/*
 * A083856  Square array T(n,k) of generalized Fibonacci numbers, read by
 *          antidiagonals upwards (n, k >= 0).
 * https://oeis.org/A083856
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 84, u16 n <= 181, u32 n <= 424, u64 n <= 1112,
 *            u128 n <= 3058, big any n
 * Validated: standard, pass, 94 terms through n = 9952 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A083856 – antidiagonal reading of the array T(n,k)
   where T(n,0)=0, T(n,1)=1, and
   T(n,k)=T(n,k‑1)+n*T(n,k‑2)   (n,k ≥ 0).
   For an index n ≥ 0:
     let d = floor((sqrt(8n+1)‑1)/2)   – the diagonal number,
     let t = n – d*(d+1)/2             – offset within the diagonal.
     then a(n) = T(d‑t , t).
   This formula follows the OEIS definition.
   No floating‑point operations are used – d is found by a simple integer loop. */

#include <stdint.h>

static inline seq_t T_of(idx_t n, idx_t k)
{
    seq_t b0 = 0;                   /* T(n,0) */
    if (k == 0) return b0;

    seq_t b1 = 1;                   /* T(n,1) */
    if (k == 1) return b1;

    seq_t nseq = (seq_t)n;          /* cast n to seq_t for multiplication */
    seq_t prev2 = b0;               /* T(n,k-2) */
    seq_t prev1 = b1;               /* T(n,k-1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= k; ++i) {
        cur = prev1 + nseq * prev2; /* recurrence */
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}

seq_t a(idx_t n)
{
    /* find diagonal d (0‑based) :  d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ( ((d + 1) * (d + 2)) / 2 <= n )
        ++d;

    /* offset t within the diagonal */
    idx_t t = n - (d * (d + 1)) / 2;

    /* coordinates in the array */
    idx_t row = d - t;   /* n in a(n,k) */
    idx_t col = t;       /* k in a(n,k) */

    return T_of(row, col);
}
