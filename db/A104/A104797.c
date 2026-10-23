/*
 * A104797  Triangle T(n,k) = Fib(n-k+4)-n-k-3, n>=1, 0<=k<n, read by rows.
 * https://oeis.org/A104797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Reconstruct the OEIS A104797 sequence.
   The triangle T(n,k) is obtained by repeatedly writing
   the sequence A001924 backwards.  A001924 satisfies
   B(1)=1, and for n≥3  
          B(n) = B(n‑1) + (B(n‑1)-B(n‑2) + F(n))
   where F(n) is the n‑th Fibonacci number (F(1)=F(2)=1).
   The flattened sequence a(n) is a(n)=B(r−offset), where
   r is the row containing the n‑th element and offset is
   its position within that row (0‑based).
*/
#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n */
    idx_t r = (idx_t)((sqrt((double)8.0 * n + 1.0) - 1.0) / 2.0);
    /* Adjust for possible rounding errors */
    while (r * (r + 1) / 2 < n) ++r;
    while (r > 0 && (r - 1) * r / 2 >= n) --r;

    /* Offset within the row (0‑based) */
    idx_t prev_terms = (r - 1) * r / 2;
    idx_t offset     = n - prev_terms - 1;          /* 0‑based */
    idx_t m          = r - offset;                  /* we need B(m) */

    /* Compute B(m) iteratively (no arrays) */
    if (m == 0) return (seq_t)0;          /* undefined, but never used */
    if (m == 1) return (seq_t)1;          /* B(1) */

    seq_t B  = (seq_t)1;                  /* B(1) */
    seq_t diff = 0;                       /* diff[1] undefined */
    seq_t fibPrev = 1; /* F(1) */
    seq_t fibCur  = 1; /* F(2) */

    /* Handle n = 2 separately */
    if (m >= 2) {
        B = (seq_t)3;   /* B(2) */
        diff = (seq_t)2;/* diff[2] = B(2)-B(1) */
        fibPrev = 1;    /* F(2) for next iteration */
        fibCur  = 1;    /* F(3) will be computed */
    }

    for (idx_t i = 3; i <= m; ++i) {
        /* Compute next Fibonacci number F(i) */
        seq_t fibNext = (seq_t)((unsigned __int128)fibPrev + fibCur);
        fibPrev = fibCur;
        fibCur  = fibNext;

        /* Update diff[i] = diff[i-1] + F(i) */
        diff += fibNext;

        /* Update B[i] = B[i-1] + diff[i] */
        B += diff;
    }

    return B;
}
