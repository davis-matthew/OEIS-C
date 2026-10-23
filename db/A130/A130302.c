/*
 * A130302  A000012 * A130296.
 * https://oeis.org/A130302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence A130302 is built row‑wise.  
   Row r (r ≥ 1) starts with the triangular number T(r)=r*(r+1)/2
   and is followed by the decreasing numbers r‑1, r‑2, … , 1.
   The function below returns the n‑th (1‑based) element of this
   concatenated sequence. */
 
#include <stdint.h>
 
/* integer square‑root: floor(sqrt(x)) for unsigned 64‑bit x */
static idx_t sqrt_floor_u64(idx_t x)
{
    idx_t low = 0, high = 0xFFFFFFFFULL;   /* 2^32-1 is the max sqrt of 64‑bit max */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid <= x / mid) {              /* mid*mid <= x */
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;                           /* floor(sqrt(x)) */
}
 
seq_t a(idx_t n)
{
    /* n is 1‑based; assume n >= 1 */
    /* solve r such that r(r+1)/2 ≥ n */
    uint64_t m = 8ULL * n + 1ULL;                 /* 8*n + 1 */
    idx_t s = sqrt_floor_u64(m);                  /* floor(sqrt(8*n+1)) */
    idx_t r = (s - 1) >> 1;                       /* floor((sqrt-1)/2) */
    if (r * (r + 1) / 2 < n) r++;                 /* ensure r is minimal enough */
    /* position inside the row */
    idx_t prev_tot = (r - 1) * r / 2;             /* total terms before row r */
    idx_t pos = n - prev_tot;                     /* 1‑based position in row r */
    if (pos == 1) {                               /* first element: triangular */
        return (seq_t)r * (seq_t)(r + 1) / 2;
    } else {                                      /* decreasing values */
        return (seq_t)(r - pos + 1);
    }
}
